/*
    fat12.h

    Benix FAT12 filesystem driver.
*/

#pragma once

#include "klib/types.h"

#define FILENAME_CHARS                  11
#define NAME_CHARS                      8
#define EXTENSION_CHARS                 3

#define ENTRY_SIZE                      32
#define SUBDIR_BYTES                    4096
#define SUBDIR_ENTRIES                  SUBDIR_BYTES / ENTRY_SIZE

#define ATTR_READ_ONLY                  0x01
#define ATTR_HIDDEN                     0x02
#define ATTR_SYSTEM                     0x04
#define ATTR_VOLUME_ID                  0x08
#define ATTR_DIRECTORY                  0x10
#define ATTR_ARCHIVE                    0x20
#define ATTR_REMOVED                    0xe5
#define ATTR_LFN                        (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

#define PATH_SEPARATOR                  '/'
#define DIR_ROOT                        "/"

// BIOS Parameter Block (BPB)
struct __attribute__((packed)) FAT12_BPB {
    u8 jmp[3];                  // jmp + nop
    u8 oem_name[8];
    u16 bytes_per_sector;
    u8 sectors_per_cluster;
    u16 reserved_sector_count;
    u8 number_of_fats;
    u16 rootdir_entry_count;
    u16 total_sectors16;
    u8 media;
    u16 fat_size;
    u16 sectors_per_track;
    u16 number_of_heads;
    u32 hidden_sectors;
    u32 total_sectors32;
    u8 drive_number;
    u8 reserved1;
    u8 boot_signature;
    u32 volume_id;
    u8 volume_label[11];
    u8 filesystem_type[8];
    u16 mbr_signature;
};

struct __attribute__((packed)) FAT12_DirectoryEntry {
    u8 name[8];
    u8 ext[3];
    u8 attributes;
    u8 reserved;
    u8 creation_time_tenth;
    u16 creation_time;
    u16 creation_date;
    u16 last_access_date;
    u16 first_cluster_high;
    u16 write_time;
    u16 write_date;
    u16 first_cluster_low;
    u32 file_size;
};

struct DriverFS_FAT12 {
    struct FAT12_BPB bpb;
    u32 fat_start_sector;
    u32 rootdir_start_sector;
    u32 data_start_sector;

    u16 (*get_next_cluster)(u16 cluster);
    void (*read_cluster)(u16 cluster, voidptr buffer);
    u32 (*read_clusters)(u16 cluster, string buffer, u32 bytes);
    u32 (*read_directory)(bool is_root, u16 cluster, struct FAT12_DirectoryEntry* out, u32 entries);
    bool (*find_entry)(bool is_root, u16 cluster, const string formatted, struct FAT12_DirectoryEntry* out);
    bool (*resolve_path)(const string path, struct FAT12_DirectoryEntry* out);

    i32 (*read_file)(const string filename, voidptr buffer, u32 buffer_size);
    i32 (*read_dir)(const string path, string buffer);
    i32 (*lookup)(const string filename);
};

void init_fsdriver_fat12(void);
struct DriverFS_FAT12* get_fsdriver_fat12(void);