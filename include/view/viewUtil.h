#ifndef VIEW_UTIL_H
#define VIEW_UTIL_H

#include <time.h>

void pauseProgram();
void clearScreen();

void getFormatTime(char *t_str, int S_size, time_t time);

#endif