#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>
#include <time.h>

/*
    FAT Entry Structure
    0bRWDAAAAAAAAAAAAA
    A: Address of next block
    R: Read permission
    W: Write permission
    D: Directory flag

    A:
    0b0001111111111111: End of file

    Total number of blocks = 4 * 1024 * 1024 / 512 = 8192
    Max address = 8191 = 0b0001111111111111

    Root directory is stored in the first block



typedef struct
{
    uint32_t totalBlocks;
    uint32_t freeBlocks;
    uint32_t blockSize;
} SuperBlock;

typedef struct
{
    uint32_t size;
    time_t creationTime;
    time_t modificationTime;
    char password[32]; // Assuming max password length of 31 characters + null terminator
    char name[];
} DirectoryEntry;


a directory consists of only one block
*/

#define FAT_ADDRESS_MASK 0b0001111111111111
#define FAT_WRITE_PERMISSION_MASK 0b1000000000000000
#define FAT_READ_PERMISSION_MASK 0b0100000000000000
#define FAT_DIR_MASK 0b0010000000000000

#define FAT_FREE 0b0000000000000000
#define FAT_END 0b0001111111111111

#define SUPER_BLOCK_ADDRESS 0
#define ROOT_DIR_ADDRESS 1
#define RESERVED_BLOCKS 2
#define RESERVED_FATS 2

#define BLOCK_SIZE_512 512
#define BLOCK_SIZE_1024 1024
#define TOTAL_BLOCKS (4 * 1024 * 1024 / BLOCK_SIZE_512)
#define FAT_SIZE (TOTAL_BLOCKS)

#define METADATA_BYTES 44
#define MAX_FILENAME_LENGTH (BLOCK_SIZE_512 - METADATA_BYTES)
#define MAX_PATH_LENGTH 2048
#define MAX_PASSWORD_LENGTH 32

#define FAT_ENTRY_BITS 16

typedef struct
{
    uint16_t FAT[FAT_SIZE];
    uint8_t dataBlocks[TOTAL_BLOCKS][BLOCK_SIZE_512];
} FileSystem;

typedef enum
{
    FS_NOT_ENOUGH_BLOCKS,
    FS_DIRECTORY_FULL,
    FS_FAT_FULL,
    FS_DIRECTORY_DOES_NOT_EXIST,
    FS_DIRECTORY_EXISTS,
    FS_FILE_EXISTS,
} FileSystemError;

extern FileSystem fs;
extern FileSystemError fsError;

void initFileSystem(int blockSize);
int loadFileSystem(const char *filename);
void saveFileSystem(const char *filename);

// Function declarations for file system operations
void dir(const char *path);
void mkdir(const char *path);
void rmdir(const char *path);
void dumpe2fs();
void writeFile(const char *path, const char *linuxFile);
void readFile(const char *path, const char *linuxFile);
void deleteFile(const char *path);
void chmodFile(const char *path, const char *permissions);
void addPassword(const char *path, const char *password);

#endif // FILESYSTEM_H
