#ifndef FILEIO_H
#define FILEIO_H
#include <stddef.h>

int saveToFile(const char *filePath, const void *data, size_t elemSize, int count);
int loadFromFile(const char *filePath, void *buffer,size_t elemSize, int maxCount, int *count);
int countRecordsInFile(const char *filePath, size_t elemSize);

#endif