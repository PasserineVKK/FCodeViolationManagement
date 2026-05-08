#ifndef VIEW_UTIL_H
#define VIEW_UTIL_H
#include <time.h>

#define UI_RESET "\x1b[0m"
#define UI_BOLD "\x1b[1m"
#define UI_DIM "\x1b[2m"
#define UI_RED "\x1b[31m"
#define UI_GREEN "\x1b[32m"
#define UI_YELLOW "\x1b[33m"
#define UI_BLUE "\x1b[34m"
#define UI_CYAN "\x1b[36m"
#define UI_ROW_ALT "\x1b[48;5;234m" //dark gray

#define UI_TABLE_BORDER UI_RESET
#define UI_TABLE_HEADER UI_BOLD UI_CYAN
#define UI_ERROR UI_BOLD UI_RED
#define UI_SUCCESS UI_BOLD UI_GREEN
#define UI_WARNING UI_BOLD UI_YELLOW
#define UI_INFO UI_BOLD UI_BLUE

void enableAnsiColors(void);
void pauseProgram();
void clearScreen();

void getFormatTime(char *t_str, int S_size, time_t time);
void uiError(const char *format, ...);
void uiWarning(const char *format, ...);
void uiSuccess(const char *format, ...);
void uiInfo(const char *format, ...);
void uiTableTitle(const char *title);

#endif
