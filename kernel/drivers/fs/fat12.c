/*
    fat12.c

    Implementations of the functions in fat12.h.
*/

#include "drivers/fs/fat12.h"
#include "drivers/ata.h"
#include "klib/memory.h"
#include "klib/string.h"
#include "klib/null.h"

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

static i32 read_file(const string filename, voidptr buffer, u32 buffer_size) {
    char formatted_filename[FILENAME_CHARS];
    format_filename(filename, formatted_filename);

    u32 rootdir_entries = fs.bpb.rootdir_entry_count;
    u32 sector = fs.rootdir_start_sector;
    u8 sector_buffer[SECTOR_SIZE];
    u32 initial_buffer_size = buffer_size;

    for (u32 i = 0; i < (rootdir_entries * 32 + SECTOR_SIZE - 1) / SECTOR_SIZE; i++) {
        get_driver_ata()->read(sector + i, (u16*)sector_buffer);

        for (u32 j = 0; j < SECTOR_SIZE; j += 32) {
            struct FAT12_DirectoryEntry* entry = (struct FAT12_DirectoryEntry*)&sector_buffer[j];

            if (entry->name[0] == 0)
                return -1;
            if ((entry->attributes & 0x0f) == 0x0f)
                continue;

            if (memcmp(entry->name, formatted_filename, 8) == 0) {
                u16 cluster = entry->first_cluster_low;
                u32 remaining = entry->file_size;
                u8* ptr = (u8*)buffer;

                while (cluster < 0xff8 && remaining > 0) {
                    u8 cluster_buffer[fs.bpb.sectors_per_cluster * fs.bpb.bytes_per_sector];
                    read_cluster(cluster, cluster_buffer);

                    u32 to_copy = (remaining < sizeof(cluster_buffer)) ? remaining : sizeof(cluster_buffer);
                    if (to_copy > buffer_size) to_copy = buffer_size;

                    memcpy(ptr, cluster_buffer, to_copy);
                    ptr += to_copy;
                    buffer_size -= to_copy;
                    remaining -= to_copy;

                    if (buffer_size == 0)
                        break;

                    cluster = get_next_cluster(cluster);
                }

                return initial_buffer_size - buffer_size;
            }
        }
    }

    return -1;
}

static i32 read_dir(string buffer) {
    u32 rootdir_entries = fs.bpb.rootdir_entry_count;
    u32 sector = fs.rootdir_start_sector;
    u8 sector_buffer[SECTOR_SIZE];
    u32 out = 0;

    for (u32 i = 0; i < (rootdir_entries * 32 + SECTOR_SIZE - 1) / SECTOR_SIZE; i++) {
        get_driver_ata()->read(sector + i, (u16*)sector_buffer);

        for (u32 j = 0; j < SECTOR_SIZE; j += 32) {
            struct FAT12_DirectoryEntry* entry = (struct FAT12_DirectoryEntry*)&sector_buffer[j];

            if (entry->name[0] == 0)
                goto done;
            if ((entry->attributes & 0x0f) == 0x0f)
                continue;

            for (u32 k = 0; k < FILENAME_CHARS; k++) {
                buffer[out++] = entry->name[k];
            }

            buffer[out++] = '\n';
        }
    }

    done:
        buffer[out] = '\0';

    return 0;
}

static i32 lookup(const string filename) {
    char formatted_filename[11];
    format_filename(filename, formatted_filename);

    u32 rootdir_entries = fs.bpb.rootdir_entry_count;
    u32 sector = fs.rootdir_start_sector;
    u8 sector_buffer[SECTOR_SIZE];
    u8 buffer[11];
    u32 out = 0;

    for (u32 i = 0; i < (rootdir_entries * 32 + SECTOR_SIZE - 1) / SECTOR_SIZE; i++) {
        get_driver_ata()->read(sector + i, (u16*)sector_buffer);

        for (u32 j = 0; j < SECTOR_SIZE; j += 32) {
            struct FAT12_DirectoryEntry* entry = (struct FAT12_DirectoryEntry*)&sector_buffer[j];

            if (entry->name[0] == 0)
                goto done;
            if ((entry->attributes & 0x0f) == 0x0f)
                continue;

            for (u32 k = 0; k < FILENAME_CHARS; k++) {
                buffer[out++] = entry->name[k];
            }
            buffer[out] = '\0';
            formatted_filename[out] = '\0';

            if (strcmp((string)buffer, formatted_filename) == 0) {
                return 1;
            }

            out = 0;
        }
    }

    done:
    return 0;
}

void init_fsdriver_fat12(void) {
    fs.get_next_cluster = get_next_cluster;
    fs.read_cluster = read_cluster;
    fs.read_file = read_file;
    fs.read_dir = read_dir;
    fs.lookup = lookup;

    u8 sector[512];
    get_driver_ata()->read(0, (u16*)sector);
    memcpy(&fs.bpb, sector, sizeof(fs.bpb));

    fs.fat_start_sector = fs.bpb.reserved_sector_count;
    fs.rootdir_start_sector = fs.fat_start_sector + fs.bpb.number_of_fats * fs.bpb.fat_size;
    fs.data_start_sector = fs.rootdir_start_sector + ((fs.bpb.rootdir_entry_count * 32) + (fs.bpb.bytes_per_sector - 1)) / fs.bpb.bytes_per_sector;
}

struct DriverFS_FAT12* get_fsdriver_fat12(void) {
    return &fs;
}