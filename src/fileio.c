#include <stdio.h>
#include "../include/fileio.h"

int saveToFile(const char *filePath, const void *data, size_t elemSize, int count) {
    FILE *fp = fopen(filePath, "wb");
    if (fp == NULL) {
        printf("Cannot open '%s' for writing.\n", filePath);
        return 0;
    }
    int written = (int)fwrite(data, elemSize, count, fp);
    fclose(fp);
    if (written != count) {
        printf("Expected to write %d records, wrote %d.\n", count, written);
        return 0;
    }
    return 1;
}

int loadFromFile(const char *filePath, void *buffer, size_t elemSize, int maxCount, int *count) {
    *count = 0;
    FILE *fp = fopen(filePath, "rb");
    if (fp == NULL)  return -1; // File not found
    *count = (int)fread(buffer, elemSize, maxCount, fp);
    fclose(fp);
    return *count;
}
