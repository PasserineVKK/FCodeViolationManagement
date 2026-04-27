#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "view/viewUtil.h"

#ifdef _WIN32
#define OS 0
#elif __linux
#define OS 1
#endif

void pauseProgram()
{
    if (OS == 0)
        system("pause");
    else if (OS == 1)
        system("read -p \" Press enter to continue \" ");
}

void clearScreen()
{
    if (OS == 0)
        system("cls");
    else if (OS == 1)
        system("clear");
}

void getFormatTime(char *t_str, int size, time_t time)
{
    struct tm *timeInfo;
    timeInfo = localtime(&time);
    strftime(t_str, size, "%Y-%m-%d %H:%M", timeInfo);
}
