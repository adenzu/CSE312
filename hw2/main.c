#include <stdio.h>
#include <stdlib.h>
#include "filesystem.h"

void createFileSystem(int blockSize, const char *filename)
{
    initFileSystem(blockSize);

    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("Failed to create file system");
        exit(EXIT_FAILURE);
    }
    fwrite(&fs, sizeof(FileSystem), 1, file);
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <block_size_kb> <filesystem_filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *blockSizeStr = argv[1];
    const char *filename = argv[2];

    float blockSizeKB = atof(blockSizeStr);
    int blockSize = blockSizeKB == 0.5 ? BLOCK_SIZE_512 : blockSizeKB == 1.0 ? BLOCK_SIZE_1024
                                                                             : 0;

    if (blockSize != BLOCK_SIZE_512 && blockSize != BLOCK_SIZE_1024)
    {
        fprintf(stderr, "Supported block sizes are 0.5 KB and 1 KB\n");
        return EXIT_FAILURE;
    }

    createFileSystem(blockSize, filename);
    printf("File system created successfully\n");

    return EXIT_SUCCESS;
}
