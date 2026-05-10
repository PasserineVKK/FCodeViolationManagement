#include "../../include/view/viewUtil.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#define OS 0
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#elif __linux
#define OS 1
#endif

//Enable ANSI colors for Windows
void enableAnsiColors(void) {
#ifdef _WIN32
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;

    if (output == INVALID_HANDLE_VALUE) return;
    if (!GetConsoleMode(output, &mode)) return;

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(output, mode);
#endif
}

void pauseProgram() {
    if (OS == 0)
        system("pause");
    else if (OS == 1)
        system("read -p \" Press enter to continue \" ");
}

void clearScreen() {
    if (OS == 0)
        system("cls");
    else if (OS == 1)
        system("clear");
}

void getFormatTime(char* t_str, int size, time_t time) {
    struct tm* timeInfo;
    timeInfo = localtime(&time);
    strftime(t_str, size, "%H:%M %d-%m-%Y", timeInfo);
}

static void uiPrintWithStyle(const char* style, const char* format, va_list args) {
    printf("%s", style);
    vprintf(format, args);
    printf("%s", UI_RESET);
}

void uiError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    uiPrintWithStyle(UI_ERROR, format, args);
    va_end(args);
}

void uiWarning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    uiPrintWithStyle(UI_WARNING, format, args);
    va_end(args);
}

void uiSuccess(const char* format, ...) {
    va_list args;
    va_start(args, format);
    uiPrintWithStyle(UI_SUCCESS, format, args);
    va_end(args);
}

void uiInfo(const char* format, ...) {
    va_list args;
    va_start(args, format);
    uiPrintWithStyle(UI_INFO, format, args);
    va_end(args);
}

void uiTableTitle(const char* title) {
    printf("\n%s%s%s\n", UI_TABLE_HEADER, title, UI_RESET);
}

int uiPromptNextPage(int shownCount, int totalCount) {
    if (shownCount >= totalCount) return 0;

    char buf[32];
    printf("%s(%d/%d) Press Enter for next page, or 'q' then Enter to stop: %s",
           UI_DIM, shownCount, totalCount, UI_RESET);

    if (fgets(buf, (int)sizeof(buf), stdin) == NULL) return 0;

    // If previous code used scanf, there may be a leftover newline; treat empty as continue.
    if (buf[0] == '\n' || buf[0] == '\0') return 1;
    if (buf[0] == 'q' || buf[0] == 'Q') return 0;
    return 1;
}



void displayCountdown(int seconds) {
    while (seconds >= 0) {
        int mins = seconds / 60;
        int secs = seconds % 60;

        // screen pointer change to line:
        uiWarning("\r%02d:%02d \n ", mins, secs);
        fflush(stdout);  // push to screen

        #ifdef _WIN32
            Sleep(1000); // milisecond of win
        #else
            sleep(1);    // second of linux
        #endif

        seconds--;
    }

}
