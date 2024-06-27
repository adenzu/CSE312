#include "filesystem.h"
#include <string.h>
#include <stdio.h>

FileSystem fs;
FileSystemError fsError;

void setFATEntry(uint16_t address, uint16_t value)
{
    fs.FAT[address] = value;
}

uint16_t getFATEntry(uint16_t address)
{
    return fs.FAT[address];
}

void printDataBlock(uint16_t address)
{
    int rowLength = 16;
    for (int i = 0; i < BLOCK_SIZE_512; i++)
    {
        if (i % rowLength == 0)
        {
            printf("\n");
        }
        printf("%02X ", fs.dataBlocks[address][i]);
    }
    printf("\n");
}

void setSuperBlockTotalBlocks(uint32_t totalBlocks)
{
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 0] = totalBlocks & 0xFF;
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 1] = (totalBlocks >> 8) & 0xFF;
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 2] = (totalBlocks >> 16) & 0xFF;
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 3] = (totalBlocks >> 24) & 0xFF;
}

void setSuperBlockFreeBlocks(uint32_t freeBlocks)
{
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 4] = freeBlocks & 0xFF;
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 5] = (freeBlocks >> 8) & 0xFF;
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 6] = (freeBlocks >> 16) & 0xFF;
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 7] = (freeBlocks >> 24) & 0xFF;
}

void setSuperBlockBlockSize(uint32_t blockSize)
{
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 8] = blockSize & 0xFF;
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 9] = (blockSize >> 8) & 0xFF;
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 10] = (blockSize >> 16) & 0xFF;
    fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 11] = (blockSize >> 24) & 0xFF;
}

void setSuperBlock(uint32_t totalBlocks, uint32_t freeBlocks, uint32_t blockSize)
{
    setSuperBlockTotalBlocks(totalBlocks);
    setSuperBlockFreeBlocks(freeBlocks);
    setSuperBlockBlockSize(blockSize);
}

uint32_t getSuperBlockTotalBlocks()
{
    return fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 0] | (fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 1] << 8) | (fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 2] << 16) | (fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 3] << 24);
}

uint32_t getSuperBlockFreeBlocks()
{
    return fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 4] | (fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 5] << 8) | (fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 6] << 16) | (fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 7] << 24);
}

uint32_t getSuperBlockBlockSize()
{
    return fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 8] | (fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 9] << 8) | (fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 10] << 16) | (fs.dataBlocks[SUPER_BLOCK_ADDRESS][BLOCK_SIZE_512 - 11] << 24);
}

void getSuperBlock(uint32_t *totalBlocks, uint32_t *freeBlocks, uint32_t *blockSize)
{
    *totalBlocks = getSuperBlockTotalBlocks();
    *freeBlocks = getSuperBlockFreeBlocks();
    *blockSize = getSuperBlockBlockSize();
}

void setDirectoryEntrySize(uint16_t address, uint32_t size)
{
    fs.dataBlocks[address][0] = size & 0xFF;
    fs.dataBlocks[address][1] = (size >> 8) & 0xFF;
    fs.dataBlocks[address][2] = (size >> 16) & 0xFF;
    fs.dataBlocks[address][3] = (size >> 24) & 0xFF;
}

void setDirectoryEntryCreationTime(uint16_t address, time_t creationTime)
{
    fs.dataBlocks[address][4] = creationTime & 0xFF;
    fs.dataBlocks[address][5] = (creationTime >> 8) & 0xFF;
    fs.dataBlocks[address][6] = (creationTime >> 16) & 0xFF;
    fs.dataBlocks[address][7] = (creationTime >> 24) & 0xFF;
}

void setDirectoryEntryModificationTime(uint16_t address, time_t modificationTime)
{
    fs.dataBlocks[address][8] = modificationTime & 0xFF;
    fs.dataBlocks[address][9] = (modificationTime >> 8) & 0xFF;
    fs.dataBlocks[address][10] = (modificationTime >> 16) & 0xFF;
    fs.dataBlocks[address][11] = (modificationTime >> 24) & 0xFF;
}

void setDirectoryEntryPassword(uint16_t address, const char *password)
{
    strncpy((char *)&fs.dataBlocks[address][12], password, 32);
}

void setDirectoryEntryName(uint16_t address, const char *name)
{
    for (int i = 0; i < MAX_FILENAME_LENGTH; i++)
    {
        fs.dataBlocks[address][METADATA_BYTES + i] = name[i];
        if (name[i] == '\0')
        {
            break;
        }
    }
}

void setDirectoryEntry(uint16_t address, const char *name, uint32_t size, time_t creationTime, time_t modificationTime, const char *password)
{
    setDirectoryEntrySize(address, size);
    setDirectoryEntryCreationTime(address, creationTime);
    setDirectoryEntryModificationTime(address, modificationTime);
    setDirectoryEntryPassword(address, password);
    setDirectoryEntryName(address, name);
}

void getDirectoryEntrySize(uint16_t address, uint32_t *size)
{
    *size = fs.dataBlocks[address][0] | (fs.dataBlocks[address][1] << 8) | (fs.dataBlocks[address][2] << 16) | (fs.dataBlocks[address][3] << 24);
}

void getDirectoryEntryCreationTime(uint16_t address, time_t *creationTime)
{
    *creationTime = fs.dataBlocks[address][4] | (fs.dataBlocks[address][5] << 8) | (fs.dataBlocks[address][6] << 16) | (fs.dataBlocks[address][7] << 24);
}

void getDirectoryEntryModificationTime(uint16_t address, time_t *modificationTime)
{
    *modificationTime = fs.dataBlocks[address][8] | (fs.dataBlocks[address][9] << 8) | (fs.dataBlocks[address][10] << 16) | (fs.dataBlocks[address][11] << 24);
}

void getDirectoryEntryPassword(uint16_t address, char *password)
{
    strncpy(password, (char *)&fs.dataBlocks[address][12], 32);
}

void getDirectoryEntryName(uint16_t address, char *name)
{
    strncpy(name, (char *)&fs.dataBlocks[address][44], MAX_FILENAME_LENGTH);
}

void getDirectoryEntry(uint16_t address, char *name, uint32_t *size, time_t *creationTime, time_t *modificationTime, char *password)
{
    getDirectoryEntrySize(address, size);
    getDirectoryEntryCreationTime(address, creationTime);
    getDirectoryEntryModificationTime(address, modificationTime);
    getDirectoryEntryPassword(address, password);
    getDirectoryEntryName(address, name);
}

//---

int getHeadDataBlockContentStart(const uint8_t *dataBlock)
{
    int i = METADATA_BYTES;
    while (dataBlock[i] != '\0') // pass the entry name
    {
        i++;
    }
    i++; // pass the null terminator
    return i + 1;
}

uint16_t getNextSubdirectoryFAT(const uint8_t *dataBlock, int *blockOffset)
{
    int i = *blockOffset + 2;
    for (uint16_t entryFAT = dataBlock[i] | (dataBlock[i + 1] << 8); i < BLOCK_SIZE_512; i += 2)
    {
        if (entryFAT & FAT_DIR_MASK)
        {
            *blockOffset = i;
            return entryFAT;
        }
    }
    return 0xFFFF; // -1;
}

uint16_t getFirstSubdirectoryFAT(const uint8_t *dataBlock, int *blockOffset)
{
    int i = METADATA_BYTES;
    while (dataBlock[i] != '\0') // pass the entry name
    {
        i++;
    }
    i++;          // pass the null terminator
    while (i % 2) // it takes 16bits to index the fat
    {
        i++;
    }
    uint16_t entryFAT;
    for (; i < BLOCK_SIZE_512; i += 2)
    {
        entryFAT = dataBlock[i] | (dataBlock[i + 1] << 8);
        if (entryFAT & FAT_DIR_MASK)
        {
            *blockOffset = i;
            return entryFAT;
        }
    }
    return 0xFFFF; // -1;
}

uint16_t getNextEntryFAT(const uint8_t *dataBlock, int *blockOffset)
{
    int i = *blockOffset + 2;
    if (i >= BLOCK_SIZE_512)
    {
        return 0xFFFF;
    }
    uint16_t entryFAT = dataBlock[i] | (dataBlock[i + 1] << 8);
    *blockOffset = i;
    return entryFAT;
}

uint16_t getFirstEntryFAT(const uint8_t *dataBlock, int *blockOffset)
{
    int i = METADATA_BYTES;
    while (dataBlock[i] != '\0') // pass the entry name
    {
        i++;
    }
    i++;          // pass the null terminator
    while (i % 2) // FAT entries are 16bits
    {
        i++;
    }
    if (i >= BLOCK_SIZE_512)
    {
        return 0xFFFF;
    }
    uint16_t entryFAT = dataBlock[i] | (dataBlock[i + 1] << 8);
    *blockOffset = i;
    return entryFAT;
}

int getEntryOffset(const uint8_t *dataBlock, const char *name)
{
    int offset = 0;
    uint16_t entryFAT = getFirstEntryFAT(dataBlock, &offset);

    if (entryFAT == 0xFFFF)
    {
        return -1;
    }

    char entryName[MAX_FILENAME_LENGTH];
    getDirectoryEntryName(entryFAT & FAT_ADDRESS_MASK, entryName);

    while (strncmp(name, entryName, MAX_FILENAME_LENGTH) != 0)
    {
        entryFAT = getNextEntryFAT(dataBlock, &offset);
        if (entryFAT == 0xFFFF)
        {
            return -1;
        }
        getDirectoryEntryName(entryFAT & FAT_ADDRESS_MASK, entryName);
    }

    return offset;
}

uint16_t getFileFAT(const uint8_t *dataBlock, const char *name)
{
    int offset = getEntryOffset(dataBlock, name);
    if (offset == -1)
    {
        return 0xFFFF;
    }
    return dataBlock[offset] | (dataBlock[offset + 1] << 8);
}

int getDirectoryAvailableSpot(const uint8_t *dataBlock)
{
    int i = METADATA_BYTES;
    while (dataBlock[i] != '\0') // pass the entry name
    {
        i++;
    }
    i++;          // pass the null terminator
    while (i % 2) // it takes 16bits to index the fat
    {
        i++;
    }
    uint16_t entryFAT;
    for (; i < BLOCK_SIZE_512; i += 2)
    {
        entryFAT = dataBlock[i] | (dataBlock[i + 1] << 8);
        if (entryFAT == FAT_FREE)
        {
            return i;
        }
    }
    return -1;
}

int isDirectoryEmpty(const uint8_t *directoryDataBlock)
{
    int i = METADATA_BYTES;
    while (directoryDataBlock[i] != '\0') // pass the entry name
    {
        i++;
    }
    i++;          // pass the null terminator
    while (i % 2) // it takes 16bits to index the fat
    {
        i++;
    }
    uint16_t entryFAT;
    for (; i < BLOCK_SIZE_512; i += 2)
    {
        entryFAT = directoryDataBlock[i] | (directoryDataBlock[i + 1] << 8);
        if (entryFAT != FAT_FREE)
        {
            return 0;
        }
    }
    return 1;
}

int putFAT(uint8_t *parentDirectoryDataBlock, int offset, uint16_t FATIndex)
{
    if (offset >= BLOCK_SIZE_512)
    {
        return -1;
    }
    parentDirectoryDataBlock[offset] = FATIndex & 0xFF;
    parentDirectoryDataBlock[offset + 1] = (FATIndex >> 8) & 0xFF;
    return 0;
}

int updateFAT(uint8_t *parentDirectoryDataBlock, uint16_t newFAT)
{
    int offset;
    uint16_t entryFAT = getFirstEntryFAT(parentDirectoryDataBlock, &offset);

    while (entryFAT != 0xFFFF)
    {
        if ((entryFAT & FAT_ADDRESS_MASK) == (newFAT & FAT_ADDRESS_MASK))
        {
            putFAT(parentDirectoryDataBlock, offset, newFAT);
            return 0;
        }
        entryFAT = getNextEntryFAT(parentDirectoryDataBlock, &offset);
    }
    return -1;
}

void freeFAT(uint8_t *dataBlock, int offset)
{
    if (offset >= BLOCK_SIZE_512)
    {
        return;
    }
    dataBlock[offset] = FAT_FREE;
    dataBlock[offset + 1] = (FAT_FREE >> 8) & 0xFF;
}

void deleteFromDirectory(uint8_t *directoryDataBlock, const char *name)
{
    int offset = getEntryOffset(directoryDataBlock, name);
    if (offset == -1)
    {
        return;
    }
    freeFAT(directoryDataBlock, offset);
}

void parsePathIntoFirstAndRest(const char *path, char *first, char *rest)
{
    int firstSlashIndex = 0;
    for (int i = 1; path[i] != '\0'; i++)
    {
        if (path[i] == '/')
        {
            firstSlashIndex = i;
            break;
        }
    }

    if (firstSlashIndex == 0)
    {
        strncpy(first, path, MAX_PATH_LENGTH);
        rest[0] = '\0';
        return;
    }

    strncpy(first, path, firstSlashIndex);
    first[firstSlashIndex] = '\0';

    strncpy(rest, &path[firstSlashIndex], MAX_PATH_LENGTH);
}

uint16_t getDirectoryFATHelper(const char *directoryPath, uint16_t currentDirectoryFAT)
{
    if (!(currentDirectoryFAT & FAT_DIR_MASK))
    {
        return 0xFFFF; // -1
    }

    char first[MAX_PATH_LENGTH];
    char rest[MAX_PATH_LENGTH];
    parsePathIntoFirstAndRest(directoryPath, first, rest);

    uint8_t *block = fs.dataBlocks[currentDirectoryFAT & FAT_ADDRESS_MASK];

    int blockOffset = 0;
    uint16_t subdirectoryFAT = getFirstSubdirectoryFAT(block, &blockOffset);

    if (subdirectoryFAT == 0xFFFF)
    {
        return 0xFFFF; // -1
    }

    char entryName[MAX_FILENAME_LENGTH];
    getDirectoryEntryName(subdirectoryFAT & FAT_ADDRESS_MASK, entryName);

    while (strcmp(&first[1], entryName) != 0)
    {
        subdirectoryFAT = getNextSubdirectoryFAT(block, &blockOffset);
        if (subdirectoryFAT == 0xFFFF)
        {
            return 0xFFFF;
        }
        getDirectoryEntryName(subdirectoryFAT & FAT_ADDRESS_MASK, entryName);
    }

    if (rest[0] == '\0')
    {
        return subdirectoryFAT;
    }

    return getDirectoryFATHelper((const char *)&rest, subdirectoryFAT);
}

uint16_t getDirectoryFAT(const char *directoryPath)
{
    if (directoryPath[0] != '/')
    {
        return 0xFFFF; // -1
    }
    if (directoryPath[0] == '/' && directoryPath[1] == '\0')
    {
        return ROOT_DIR_ADDRESS;
    }
    return getDirectoryFATHelper(directoryPath, ROOT_DIR_ADDRESS | FAT_DIR_MASK | FAT_READ_PERMISSION_MASK);
}

uint16_t createDirectory(const char *parentDirectory, const char *name, uint16_t permissions)
{
    if (getSuperBlockFreeBlocks() <= 0)
    {
        fsError = FS_NOT_ENOUGH_BLOCKS;
        return 0xFFFF;
    }

    uint16_t directoryFAT = getDirectoryFAT(parentDirectory);

    if (directoryFAT == 0xFFFF)
    {
        fsError = FS_DIRECTORY_DOES_NOT_EXIST;
        return 0xFFFF;
    }

    uint8_t *parentDirectoryBlock = fs.dataBlocks[directoryFAT & FAT_ADDRESS_MASK];

    if (getEntryOffset(parentDirectoryBlock, name) != -1)
    {
        fsError = FS_DIRECTORY_EXISTS;
        return 0xFFFF;
    }

    int offset = getDirectoryAvailableSpot(parentDirectoryBlock);
    if (offset == -1) // parent directory reached its limit of files to store
    {
        fsError = FS_DIRECTORY_FULL;
        return 0xFFFF;
    }

    for (uint16_t i = RESERVED_FATS; i < FAT_SIZE; i++)
    {
        if (fs.FAT[i] == FAT_FREE)
        {
            setFATEntry(i, FAT_END | FAT_DIR_MASK | permissions);
            putFAT(parentDirectoryBlock, offset, i | FAT_DIR_MASK | permissions);
            setDirectoryEntry(i, name, 0, time(NULL), time(NULL), "");
            setSuperBlockFreeBlocks(getSuperBlockFreeBlocks() - 1);
            return i | FAT_DIR_MASK | permissions;
        }
    }

    fsError = FS_FAT_FULL;
    return 0xFFFF;
}

uint16_t createFile(const char *parentDirectory, const char *name, uint16_t permissions)
{
    if (getSuperBlockFreeBlocks() <= 0)
    {
        fsError = FS_NOT_ENOUGH_BLOCKS;
        return 0xFFFF;
    }

    uint16_t directoryFAT = getDirectoryFAT(parentDirectory);

    if (directoryFAT == 0XFFFF)
    {
        fsError = FS_DIRECTORY_DOES_NOT_EXIST;
        return 0xFFFF;
    }

    uint8_t *parentDirectoryBlock = fs.dataBlocks[directoryFAT & FAT_ADDRESS_MASK];

    uint16_t existingFileFat = getFileFAT(parentDirectoryBlock, name);

    if (existingFileFat != 0xFFFF)
    {
        fsError = FS_FILE_EXISTS;
        return 0xFFFF;
    }

    int offset = getDirectoryAvailableSpot(parentDirectoryBlock);
    if (offset == -1) // parent directory reached its limit of files to store
    {
        fsError = FS_DIRECTORY_FULL;
        return 0xFFFF;
    }

    for (uint16_t i = RESERVED_FATS; i < FAT_SIZE; i++)
    {
        if (fs.FAT[i] == FAT_FREE)
        {
            setFATEntry(i, FAT_END | permissions);
            putFAT(parentDirectoryBlock, offset, i | permissions);
            setDirectoryEntry(i, name, 0, time(NULL), time(NULL), "");
            return i | permissions;
        }
    }

    fsError = FS_FAT_FULL;
    return 0xFFFF;
}

char *getTimeString(time_t time)
{
    static char timeString[26];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localtime(&time));
    return timeString;
}

char *getPermissionString(uint16_t entryFAT)
{
    static char permissionString[4];
    permissionString[0] = (entryFAT & FAT_DIR_MASK) ? 'd' : '-';
    permissionString[1] = (entryFAT & FAT_READ_PERMISSION_MASK) ? 'r' : '-';
    permissionString[2] = (entryFAT & FAT_WRITE_PERMISSION_MASK) ? 'w' : '-';
    permissionString[3] = '\0';
    return permissionString;
}

void parsePath(const char *path, char *parentDirectory, char *name)
{
    int lastSlashIndex = 0;
    for (int i = 0; path[i] != '\0'; i++)
    {
        if (path[i] == '/')
        {
            lastSlashIndex = i;
        }
    }

    if (lastSlashIndex == 0)
    {
        parentDirectory[0] = '/';
        parentDirectory[1] = '\0';
        strncpy(name, &path[1], MAX_FILENAME_LENGTH);
        return;
    }

    strncpy(parentDirectory, path, lastSlashIndex);
    parentDirectory[lastSlashIndex] = '\0';

    strncpy(name, &path[lastSlashIndex + 1], MAX_FILENAME_LENGTH);
}

void printEntryInfo(uint16_t entryFAT)
{
    char name[MAX_FILENAME_LENGTH];
    uint32_t size;
    time_t creationTime;
    time_t modificationTime;
    char password[MAX_PASSWORD_LENGTH];

    getDirectoryEntry(entryFAT & FAT_ADDRESS_MASK, name, &size, &creationTime, &modificationTime, password);

    printf("%s ", getPermissionString(entryFAT));
    printf("%10u  ", size);
    printf("%s  ", getTimeString(creationTime));
    printf("%s  ", getTimeString(modificationTime));
    printf("%-32s  ", password);
    printf("%s\n", name);
}

uint16_t findFreeDataBlock()
{
    for (uint16_t i = RESERVED_FATS; i < FAT_SIZE; i++)
    {
        if (fs.FAT[i] == FAT_FREE)
        {
            return i;
        }
    }
    return 0xFFFF;
}

void freeDataBlock(uint16_t address)
{
    setFATEntry(address, FAT_FREE);
    memset(fs.dataBlocks[address], 0, BLOCK_SIZE_512);
    setSuperBlockFreeBlocks(getSuperBlockFreeBlocks() + 1);
}

void freeDataBlocks(uint16_t address)
{
    uint16_t nextAddress = fs.FAT[address] & FAT_ADDRESS_MASK;
    freeDataBlock(address);
    while (nextAddress != FAT_END)
    {
        address = nextAddress;
        nextAddress = fs.FAT[address] & FAT_ADDRESS_MASK;
        freeDataBlock(address);
    }
}

int checkForPassword(uint16_t fileDataBlockAddress)
{
    char password[MAX_PASSWORD_LENGTH];
    getDirectoryEntryPassword(fileDataBlockAddress, password);

    if (password[0] == '\0')
    {
        return 1;
    }

    printf("This file is secured with a password. Enter its password to continue your operation: ");

    char enteredPassword[MAX_PASSWORD_LENGTH];
    for (int i = 0; i < MAX_PASSWORD_LENGTH; i++)
    {
        enteredPassword[i] = getchar();
        if (enteredPassword[i] == '\n')
        {
            enteredPassword[i] = '\0';
            break;
        }
    }

    return strcmp(password, enteredPassword) == 0;
}

int checkForReadPermission(uint16_t fileFAT)
{
    return (fileFAT & FAT_READ_PERMISSION_MASK) == FAT_READ_PERMISSION_MASK;
}

int checkForWritePermission(uint16_t fileFAT)
{
    return (fileFAT & FAT_WRITE_PERMISSION_MASK) == FAT_WRITE_PERMISSION_MASK;
}

// ---

void initFileSystem(int blockSize)
{
    // Initialize FAT table
    for (int i = 0; i < FAT_SIZE; i++)
    {
        fs.FAT[i] = FAT_FREE;
    }

    // Initialize data blocks
    for (int i = 0; i < TOTAL_BLOCKS; i++)
    {
        memset(fs.dataBlocks[i], 0, BLOCK_SIZE_512);
    }

    // Initialize super block
    setFATEntry(SUPER_BLOCK_ADDRESS, FAT_END | FAT_READ_PERMISSION_MASK);
    setSuperBlock(TOTAL_BLOCKS, TOTAL_BLOCKS - 2, blockSize);
    setDirectoryEntry(SUPER_BLOCK_ADDRESS, "super_block\0", 0, time(NULL), time(NULL), "");

    // Initialize root directory
    setFATEntry(ROOT_DIR_ADDRESS, FAT_END | FAT_DIR_MASK | FAT_READ_PERMISSION_MASK | FAT_WRITE_PERMISSION_MASK);
    setDirectoryEntry(ROOT_DIR_ADDRESS, "root\0", 0, time(NULL), time(NULL), "");
}

int loadFileSystem(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("Failed to open file system");
        return -1;
    }
    fread(&fs, sizeof(FileSystem), 1, file);
    fclose(file);
    return 0;
}

void saveFileSystem(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("Failed to save file system");
        return;
    }
    fwrite(&fs, sizeof(FileSystem), 1, file);
    fclose(file);
}

// ---

void dir(const char *path)
{
    uint16_t directoryFAT = getDirectoryFAT(path);

    if (directoryFAT == 0xFFFF)
    {
        printf("Given directory path does not exist.\n");
        return;
    }

    int offset = 0;
    uint8_t *directoryBlock = fs.dataBlocks[directoryFAT & FAT_ADDRESS_MASK];

    uint16_t entryFAT = getFirstEntryFAT(directoryBlock, &offset);

    while (entryFAT != 0xFFFF)
    {
        if (entryFAT != FAT_FREE)
        {
            printEntryInfo(entryFAT);
        }
        entryFAT = getNextEntryFAT(directoryBlock, &offset);
    }

    // printDataBlock(directoryFAT & FAT_ADDRESS_MASK);
}

void mkdir(const char *path)
{
    char parentDirectory[MAX_PATH_LENGTH];
    char name[MAX_FILENAME_LENGTH];

    parsePath(path, parentDirectory, name);

    uint16_t success = createDirectory(parentDirectory, name, FAT_READ_PERMISSION_MASK | FAT_WRITE_PERMISSION_MASK);

    if (success == 0xFFFF)
    {
        printf("Failed to create directory. ");
        switch (fsError)
        {
        case FS_NOT_ENOUGH_BLOCKS:
            printf("Not enough blocks to create directory.\n");
            break;
        case FS_DIRECTORY_FULL:
            printf("Parent directory is full.\n");
            break;
        case FS_FAT_FULL:
            printf("FAT table is full.\n");
            break;
        case FS_DIRECTORY_DOES_NOT_EXIST:
            printf("Parent directory does not exist.\n");
            break;
        case FS_DIRECTORY_EXISTS:
            printf("Directory already exists.\n");
            break;
        default:
            break;
        }
    }
    else
    {
        printf("Directory created successfully.\n");
    }
}

void deleteFile(const char *path)
{
    char parentDirectory[MAX_PATH_LENGTH];
    char name[MAX_FILENAME_LENGTH];

    parsePath(path, parentDirectory, name);

    uint16_t parentDirectoryFAT = getDirectoryFAT(parentDirectory);

    if (parentDirectoryFAT == 0xFFFF)
    {
        printf("Failed to delete file. Parent directory does not exist.\n");
        return;
    }

    int offset = 0;
    uint8_t *parentDirectoryBlock = fs.dataBlocks[parentDirectoryFAT & FAT_ADDRESS_MASK];

    uint16_t fileFAT = getFirstEntryFAT(parentDirectoryBlock, &offset);

    if (fileFAT == 0xFFFF)
    {
        printf("Failed to delete file. File does not exist.\n");
        return;
    }

    char entryName[MAX_FILENAME_LENGTH];
    getDirectoryEntryName(fileFAT & FAT_ADDRESS_MASK, entryName);

    while (strncmp(name, entryName, MAX_FILENAME_LENGTH) != 0)
    {
        fileFAT = getNextEntryFAT(parentDirectoryBlock, &offset);
        if (fileFAT == 0xFFFF)
        {
            printf("Failed to delete file. File does not exist.\n");
            return;
        }
        getDirectoryEntryName(fileFAT & FAT_ADDRESS_MASK, entryName);
    }

    if (!checkForPassword(fileFAT & FAT_ADDRESS_MASK))
    {
        printf("Wrong password. File is not deleted.\n");
        return;
    }

    freeFAT(parentDirectoryBlock, offset);
    freeDataBlocks(fileFAT & FAT_ADDRESS_MASK);

    printf("File deleted successfully.\n");
}

void rmdir(const char *path)
{
    if (path[0] == '/' && path[1] == '\0')
    {
        printf("Failed to delete directory. Root directory cannot be deleted.\n");
        return;
    }

    char parentDirectory[MAX_PATH_LENGTH];
    char name[MAX_FILENAME_LENGTH];

    parsePath(path, parentDirectory, name);

    uint16_t parentDirectoryFAT = getDirectoryFAT(parentDirectory);

    if (parentDirectoryFAT == 0xFFFF)
    {
        printf("Failed to delete directory. Parent directory does not exist.\n");
        return;
    }

    uint16_t directoryFAT = getDirectoryFAT(path);

    if (directoryFAT == 0xFFFF)
    {
        printf("Failed to delete directory. Directory does not exist.\n");
        return;
    }

    uint8_t *directoryBlock = fs.dataBlocks[directoryFAT & FAT_ADDRESS_MASK];

    if (!isDirectoryEmpty(directoryBlock))
    {
        printf("Failed to delete directory. Directory is not empty.\n");
        return;
    }

    deleteFromDirectory(fs.dataBlocks[parentDirectoryFAT & FAT_ADDRESS_MASK], name);
    setFATEntry(directoryFAT & FAT_ADDRESS_MASK, FAT_FREE);
    memset(directoryBlock, 0, BLOCK_SIZE_512);

    printf("Directory deleted successfully.\n");
}

void dumpe2fs()
{
    uint32_t totalBlocks;
    uint32_t freeBlocks;
    uint32_t blockSize;

    getSuperBlock(&totalBlocks, &freeBlocks, &blockSize);

    printf("Total blocks: %u\n", totalBlocks / (blockSize / BLOCK_SIZE_512));
    printf("Free blocks: %u\n", freeBlocks / (blockSize / BLOCK_SIZE_512));
    printf("Block size: %u\n", blockSize);
    printf("\n");

    uint8_t continuationOf[FAT_SIZE] = {0};

    printf("FAT table:\n");
    for (int i = 0; i < FAT_SIZE; i++)
    {
        if (continuationOf[i])
        {
            if ((fs.FAT[i] & FAT_ADDRESS_MASK) != FAT_END)
            {
                continuationOf[fs.FAT[i] & FAT_ADDRESS_MASK] = continuationOf[i];
            }
            printf("%04X: %04X\n\t", i, fs.FAT[i]);
            printEntryInfo(continuationOf[i] | (fs.FAT[continuationOf[i]] & (FAT_READ_PERMISSION_MASK | FAT_WRITE_PERMISSION_MASK | FAT_DIR_MASK)));
            continue;
        }
        if (fs.FAT[i] != FAT_FREE)
        {
            printf("%04X: %04X\n\t", i, fs.FAT[i]);
            printEntryInfo(i | (fs.FAT[i] & (FAT_READ_PERMISSION_MASK | FAT_WRITE_PERMISSION_MASK | FAT_DIR_MASK)));
            if ((fs.FAT[i] & FAT_ADDRESS_MASK) != FAT_END)
            {
                continuationOf[fs.FAT[i] & FAT_ADDRESS_MASK] = i;
            }
        }
    }
}

void writeFile(const char *path, const char *linuxFile)
{
    FILE *file = fopen(linuxFile, "rb");

    if (!file)
    {
        perror("Failed to open file");
        return;
    }

    char parentDirectory[MAX_PATH_LENGTH];
    char name[MAX_FILENAME_LENGTH];

    parsePath(path, parentDirectory, name);

    uint16_t fileHeadFAT = createFile(parentDirectory, name, FAT_READ_PERMISSION_MASK | FAT_WRITE_PERMISSION_MASK);

    if (fileHeadFAT == 0xFFFF)
    {
        printf("Failed to create file. ");
        switch (fsError)
        {
        case FS_NOT_ENOUGH_BLOCKS:
            printf("Not enough blocks to create file.\n");
            break;
        case FS_DIRECTORY_FULL:
            printf("Parent directory is full.\n");
            break;
        case FS_FAT_FULL:
            printf("FAT table is full.\n");
            break;
        case FS_DIRECTORY_EXISTS:
            printf("Directory already exists.\n");
            break;
        case FS_FILE_EXISTS:
            printf("File already exists.\n");
            break;
        default:
            break;
        }
        fclose(file);
        return;
    }

    int fileSize = 0;

    uint16_t address = fileHeadFAT & FAT_ADDRESS_MASK; // first data block & FAT entry index

    int offset = getHeadDataBlockContentStart(fs.dataBlocks[address]);
    size_t bytesRead = fread(&fs.dataBlocks[address][offset], 1, BLOCK_SIZE_512 - offset, file);
    fileSize += bytesRead;

    setSuperBlockFreeBlocks(getSuperBlockFreeBlocks() - 1);

    while (!feof(file))
    {
        uint16_t nextAddress = findFreeDataBlock();

        if (nextAddress == 0xFFFF)
        {
            printf("Failed to write file. Not enough blocks to store data.\n");
            fclose(file);
            freeDataBlocks(fileHeadFAT & FAT_ADDRESS_MASK);
            uint8_t *parentDirectoryBlock = fs.dataBlocks[getDirectoryFAT(parentDirectory) & FAT_ADDRESS_MASK];
            freeFAT(parentDirectoryBlock, getEntryOffset(parentDirectoryBlock, name));
            return;
        }
        setFATEntry(address, nextAddress);

        bytesRead = fread(fs.dataBlocks[nextAddress], 1, BLOCK_SIZE_512, file);
        setSuperBlockFreeBlocks(getSuperBlockFreeBlocks() - 1);
        setFATEntry(nextAddress, FAT_END);

        fileSize += bytesRead;
        address = nextAddress;
    }

    fclose(file);

    setFATEntry(fileHeadFAT & FAT_ADDRESS_MASK, fs.FAT[fileHeadFAT & FAT_ADDRESS_MASK] | FAT_WRITE_PERMISSION_MASK | FAT_READ_PERMISSION_MASK);

    setDirectoryEntrySize(fileHeadFAT & FAT_ADDRESS_MASK, fileSize);

    printf("File written successfully.\n");
}

void readFile(const char *path, const char *linuxFile)
{
    char parentDirectory[MAX_PATH_LENGTH];
    char name[MAX_FILENAME_LENGTH];

    parsePath(path, parentDirectory, name);

    uint16_t parentDirectoryFAT = getDirectoryFAT(parentDirectory);

    if (parentDirectoryFAT == 0xFFFF)
    {
        printf("Failed to read file. Parent directory does not exist.\n");
        return;
    }

    int offset = 0;
    uint8_t *parentDirectoryBlock = fs.dataBlocks[parentDirectoryFAT & FAT_ADDRESS_MASK];

    uint16_t fileFAT = getFileFAT(parentDirectoryBlock, name);

    if (fileFAT == 0xFFFF)
    {
        printf("Failed to read file. File does not exist.\n");
        return;
    }

    if (!checkForReadPermission(fileFAT))
    {
        printf("This file is not permitted to be read.\n");
        return;
    }

    uint16_t address = fileFAT & FAT_ADDRESS_MASK;

    if (!checkForPassword(address))
    {
        printf("Wrong password. File is not read.\n");
        return;
    }

    FILE *file = fopen(linuxFile, "wb");

    if (!file)
    {
        perror("Failed to open file");
        return;
    }

    uint32_t fileSize;
    getDirectoryEntrySize(address, &fileSize);

    offset = getHeadDataBlockContentStart(fs.dataBlocks[address]);

    uint32_t writtenFileSize = 0;
    writtenFileSize += fwrite(&fs.dataBlocks[address][offset], 1, BLOCK_SIZE_512 - offset, file);

    address = fs.FAT[address] & FAT_ADDRESS_MASK;
    while (address != FAT_END)
    {
        if (writtenFileSize >= fileSize)
        {
            break;
        }
        if (writtenFileSize + BLOCK_SIZE_512 > fileSize)
        {
            writtenFileSize += fwrite(fs.dataBlocks[address], 1, fileSize - writtenFileSize, file);
            break;
        }
        writtenFileSize += fwrite(fs.dataBlocks[address], 1, BLOCK_SIZE_512, file);
        address = fs.FAT[address] & FAT_ADDRESS_MASK;
    }

    fclose(file);

    printf("File read successfully.\n");
}

void chmodFile(const char *path, const char *permissions)
{
    char parentDirectory[MAX_PATH_LENGTH];
    char name[MAX_FILENAME_LENGTH];

    parsePath(path, parentDirectory, name);

    uint16_t parentDirectoryFAT = getDirectoryFAT(parentDirectory);

    if (parentDirectoryFAT == 0xFFFF)
    {
        printf("Failed to change file permissions. Parent directory does not exist.\n");
        return;
    }

    uint16_t fileFAT = getFileFAT(fs.dataBlocks[parentDirectoryFAT & FAT_ADDRESS_MASK], name);

    if (fileFAT == 0xFFFF)
    {
        printf("Failed to change file permissions. File does not exist.\n");
        return;
    }

    if (!checkForPassword(fileFAT & FAT_ADDRESS_MASK))
    {
        printf("Wrong password. File's permissions are not changed.\n");
        return;
    }

    if (permissions[0] == '+')
    {
        for (int i = 1; permissions[i] != '\0'; i++)
        {
            switch (permissions[i])
            {
            case 'r':
                fileFAT |= FAT_READ_PERMISSION_MASK;
                fs.FAT[fileFAT & FAT_ADDRESS_MASK] |= FAT_READ_PERMISSION_MASK;
                break;
            case 'w':
                fileFAT |= FAT_WRITE_PERMISSION_MASK;
                fs.FAT[fileFAT & FAT_ADDRESS_MASK] |= FAT_WRITE_PERMISSION_MASK;
                break;
            default:
                printf("Invalid permission.\n");
                return;
                break;
            }
        }
    }
    else if (permissions[0] == '-')
    {
        for (int i = 1; permissions[i] != '\0'; i++)
        {
            switch (permissions[i])
            {
            case 'r':
                fileFAT &= ~FAT_READ_PERMISSION_MASK;
                fs.FAT[fileFAT & FAT_ADDRESS_MASK] &= ~FAT_READ_PERMISSION_MASK;
                break;
            case 'w':
                fileFAT &= ~FAT_WRITE_PERMISSION_MASK;
                fs.FAT[fileFAT & FAT_ADDRESS_MASK] &= ~FAT_WRITE_PERMISSION_MASK;
                break;
            default:
                printf("Invalid permission.\n");
                return;
                break;
            }
        }
    }

    setDirectoryEntryModificationTime(fileFAT & FAT_ADDRESS_MASK, time(NULL));
    updateFAT(fs.dataBlocks[parentDirectoryFAT & FAT_ADDRESS_MASK], fileFAT);

    printf("File permissions changed successfully.\n");
}

void addPassword(const char *path, const char *password)
{
    char parentDirectory[MAX_PATH_LENGTH];
    char name[MAX_FILENAME_LENGTH];

    parsePath(path, parentDirectory, name);

    uint16_t parentDirectoryFAT = getDirectoryFAT(parentDirectory);

    if (parentDirectoryFAT == 0xFFFF)
    {
        printf("Failed to add password. Parent directory does not exist.\n");
        return;
    }

    uint16_t fileFAT = getFileFAT(fs.dataBlocks[parentDirectoryFAT & FAT_ADDRESS_MASK], name);

    if (fileFAT == 0xFFFF)
    {
        printf("Failed to add password. File does not exist.\n");
        return;
    }

    setDirectoryEntryPassword(fileFAT & FAT_ADDRESS_MASK, password);
    setDirectoryEntryModificationTime(fileFAT & FAT_ADDRESS_MASK, time(NULL));

    printf("Password added successfully.\n");
}
