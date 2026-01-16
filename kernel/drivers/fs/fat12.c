/*
    fat12.c

    Implementations of the functions in fat12.h.
*/

#include "drivers/fs/fat12.h"
#include "drivers/ata.h"
#include "klib/logging.h"
#include "klib/memory.h"
#include "klib/string.h"
#include "klib/null.h"
#include "klib/bool.h"

static struct DriverFS_FAT12 fs;

static void format_filename(const string input, string out) {
    memset(out, ' ', FILENAME_CHARS);

    const string dot = strchr(input, '.');
    size_t name_len = dot ? (size_t)(dot - input) : strlen(input);
    size_t ext_len  = dot ? strlen((string)dot + 1) : 0;

    if (name_len > 8) name_len = NAME_CHARS;
    if (ext_len  > 3) ext_len  = EXTENSION_CHARS;

    for (size_t i = 0; i < name_len; i++) {
        out[i] = toupper(input[i]);
    }

    for (size_t i = 0; i < ext_len; i++) {
        out[8 + i] = toupper(dot[1 + i]);
    }
}

static u16 get_next_cluster(u16 cluster) {
    u32 fat_offset = cluster + cluster / 2;
    u8 sector[SECTOR_SIZE];
    u32 sector_number = fs.fat_start_sector + (fat_offset / fs.bpb.bytes_per_sector);
    u32 offset = fat_offset % fs.bpb.bytes_per_sector;
    u16 next_cluster;

    get_driver_ata()->read(sector_number, (u16*)sector);

    u16 value = *(u16*)(sector + offset);
    if (cluster & 0x1) {
        next_cluster = value >> 4;
    } else {
        next_cluster = value & 0xfff;
    }

    return next_cluster;
}

static void read_cluster(u16 cluster, voidptr buffer) {
    u32 sector1 = fs.data_start_sector + (cluster - 2) * fs.bpb.sectors_per_cluster;

    for (u8 i = 0; i < fs.bpb.sectors_per_cluster; i++) {
        get_driver_ata()->read(sector1 + i, buffer + i * fs.bpb.bytes_per_sector);
    }
}

static u32 read_clusters(u16 cluster, string buffer, u32 bytes) {
    u32 total = 0;

    while (cluster < 0xff8 && total < bytes) {
        u8 cluster_buffer[fs.bpb.sectors_per_cluster * fs.bpb.bytes_per_sector];
        read_cluster(cluster, cluster_buffer);

        u32 to_copy = sizeof(cluster_buffer);
        if (to_copy > bytes - total) {
            to_copy = bytes - total;
        }

        memcpy(buffer + total, cluster_buffer, to_copy);
        total += to_copy;

        cluster = get_next_cluster(cluster);
    }

    return total;
}

static u32 read_directory(bool is_root, u16 cluster, struct FAT12_DirectoryEntry* out, u32 entries) {
    u32 count = 0;

    if (is_root) {
        u32 entries = fs.bpb.rootdir_entry_count;
        u32 sectors = (entries * ENTRY_SIZE + SECTOR_SIZE - 1) / SECTOR_SIZE;
        u8 sector_buffer[SECTOR_SIZE];

        for (u32 i = 0; i < sectors && count < entries; i++) {
            get_driver_ata()->read(fs.rootdir_start_sector + i, (u16*)sector_buffer);

            for (u32 j = 0; j < SECTOR_SIZE && count < entries; j += ENTRY_SIZE) {
                struct FAT12_DirectoryEntry* e = (struct FAT12_DirectoryEntry*)&sector_buffer[j];

                if (e->name[0] == 0) {
                    return count;
                }
                if ((e->attributes & ATTR_LFN) == ATTR_LFN) {
                    continue;
                }

                out[count++] = *e;
            }
        }
    } else {
        u8 buffer[SUBDIR_BYTES];
        u32 bytes = read_clusters(cluster, (string)buffer, sizeof(buffer));

        for (u32 offset = 0; offset + ENTRY_SIZE <= bytes && count <= entries; offset += ENTRY_SIZE) {
            struct FAT12_DirectoryEntry* e = (struct FAT12_DirectoryEntry*)(buffer + offset);

            if (e->name[0] == 0) {
                break;
            }
            if ((e->attributes & ATTR_LFN) == ATTR_LFN) {
                continue;
            }

            out[count++] = *e;
        }
    }

    return count;
}

bool find_entry(bool is_root, u16 cluster, const string formatted, struct FAT12_DirectoryEntry* out) {
    struct FAT12_DirectoryEntry entries[SUBDIR_ENTRIES];
    u32 n = read_directory(is_root, cluster, entries, SUBDIR_ENTRIES);

    for (u32 i = 0; i < n; i++) {
        if (memcmp(entries[i].name, formatted, FILENAME_CHARS) == 0) {
            *out = entries[i];
            return true;
        }
    }
    return false;
}

bool resolve_path(const string path, struct FAT12_DirectoryEntry* out) {
    bool is_root = true;
    u16 cluster = 0;

    char part[12];
    string p = path;

    while (*p) {
        string q = part;
        while (*p && *p != PATH_SEPARATOR) {
            *q++ = *p++;
        }
        *q = 0;
        if (*p == PATH_SEPARATOR) {
            p++;
        }

        char formatted[FILENAME_CHARS];
        format_filename(part, formatted);

        if (!find_entry(is_root, cluster, formatted, out)) {
            return false;
        }

        is_root = false;
        cluster = out->first_cluster_low;
    }

    return true;
}

/*
    -1: file not found
    -2: is a directory
*/
static i32 read_file(const string filename, voidptr buffer, u32 buffer_size) {
    struct FAT12_DirectoryEntry entry;

    if (!resolve_path(filename, &entry)) {
        return -1;
    }
    if (entry.attributes & ATTR_DIRECTORY) {
        return -2;
    }

    u32 to_read = entry.file_size;
    if (to_read > buffer_size) {
        to_read = buffer_size;
    }

    u32 read = read_clusters(entry.first_cluster_low, buffer, to_read);
    return (i32)read;
}

/*
    -1: directory not found
    -2: is not a directory
*/
static i32 read_dir(const string path, string buffer) {
    struct FAT12_DirectoryEntry dir;
    bool is_root = false;
    u16 cluster = 0;

    if (path == NULL || strcmp(path, DIR_ROOT) == 0 || *path == 0) {
        is_root = true;
    } else {
        if (!resolve_path(path, &dir)) {
            return -1;
        }
        if (!(dir.attributes & ATTR_DIRECTORY)) {
            return -2;
        }
        cluster = dir.first_cluster_low;
    }

    struct FAT12_DirectoryEntry entries[SUBDIR_ENTRIES];
    u32 count = read_directory(is_root, cluster, entries, SUBDIR_ENTRIES);

    u32 out = 0;
    for (u32 i = 0; i < count; i++) {
        if (entries[i].name[0] == ATTR_REMOVED) {
            continue;
        }

        for (u32 j = 0; j < FILENAME_CHARS; j++) {
            buffer[out++] = entries[i].name[j];
        }
        buffer[out++] = '\n';
    }

    buffer[out] = '\0';
    return (i32)count;
}

static i32 lookup(const string filename) {
    struct FAT12_DirectoryEntry entry;
    return resolve_path(filename, &entry) ? 1 : 0;
}

void init_fsdriver_fat12(void) {
    fs.get_next_cluster = get_next_cluster;
    fs.read_cluster = read_cluster;
    fs.read_clusters = read_clusters;
    fs.read_directory = read_directory;
    fs.find_entry = find_entry;
    fs.resolve_path = resolve_path;
    fs.read_file = read_file;
    fs.read_dir = read_dir;
    fs.lookup = lookup;

    u8 sector[SECTOR_SIZE];
    get_driver_ata()->read(0, (u16*)sector);
    memcpy(&fs.bpb, sector, sizeof(fs.bpb));

    fs.fat_start_sector = fs.bpb.reserved_sector_count;
    fs.rootdir_start_sector = fs.fat_start_sector + fs.bpb.number_of_fats * fs.bpb.fat_size;
    fs.data_start_sector = fs.rootdir_start_sector + ((fs.bpb.rootdir_entry_count * 32) + (fs.bpb.bytes_per_sector - 1)) / fs.bpb.bytes_per_sector;
}

struct DriverFS_FAT12* get_fsdriver_fat12(void) {
    return &fs;
}