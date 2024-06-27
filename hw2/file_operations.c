#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <filesystem_filename> <operation> [parameters...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *fsFilename = argv[1];
    const char *operation = argv[2];

    if (loadFileSystem(fsFilename) == -1)
    {
        return EXIT_FAILURE;
    }

    if (strcmp(operation, "dir") == 0)
    {
        if (argc < 4)
        {
            dir("/");
        }
        else
        {
            const char *path = argv[3];
            dir(path);
        }
    }
    else if (strcmp(operation, "mkdir") == 0)
    {
        if (argc < 4)
        {
            fprintf(stderr, "Usage: %s <filesystem_filename> mkdir <path>\n", argv[0]);
            return EXIT_FAILURE;
        }

        const char *path = argv[3];
        mkdir(path);
    }
    else if (strcmp(operation, "rmdir") == 0)
    {
        if (argc < 4)
        {
            fprintf(stderr, "Usage: %s <filesystem_filename> rmdir <path>\n", argv[0]);
            return EXIT_FAILURE;
        }
        const char *path = argv[3];
        rmdir(path);
    }
    else if (strcmp(operation, "dumpe2fs") == 0)
    {
        dumpe2fs();
    }
    else if (strcmp(operation, "write") == 0)
    {
        if (argc < 5)
        {
            fprintf(stderr, "Usage: %s <filesystem_filename> write <path> <linux_file>\n", argv[0]);
            return EXIT_FAILURE;
        }
        const char *path = argv[3];
        const char *linuxFile = argv[4];
        writeFile(path, linuxFile);
    }
    else if (strcmp(operation, "read") == 0)
    {
        if (argc < 5)
        {
            fprintf(stderr, "Usage: %s <filesystem_filename> read <path> <linux_file>\n", argv[0]);
            return EXIT_FAILURE;
        }
        const char *path = argv[3];
        const char *linuxFile = argv[4];
        readFile(path, linuxFile);
    }
    else if (strcmp(operation, "del") == 0)
    {
        if (argc < 4)
        {
            fprintf(stderr, "Usage: %s <filesystem_filename> del <path>\n", argv[0]);
            return EXIT_FAILURE;
        }
        const char *path = argv[3];
        deleteFile(path);
    }
    else if (strcmp(operation, "chmod") == 0)
    {
        if (argc < 5)
        {
            fprintf(stderr, "Usage: %s <filesystem_filename> chmod <path> <permissions>\n", argv[0]);
            return EXIT_FAILURE;
        }
        const char *path = argv[3];
        const char *permissions = argv[4];
        chmodFile(path, permissions);
    }
    else if (strcmp(operation, "addpw") == 0)
    {
        if (argc < 5)
        {
            fprintf(stderr, "Usage: %s <filesystem_filename> addpw <path> <password>\n", argv[0]);
            return EXIT_FAILURE;
        }
        const char *path = argv[3];
        const char *password = argv[4];
        addPassword(path, password);
    }
    else
    {
        fprintf(stderr, "Unknown operation: %s\n", operation);
    }

    saveFileSystem(fsFilename);

    return EXIT_SUCCESS;
}
