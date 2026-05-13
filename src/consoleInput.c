#include "../include/consoleInput.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/validate.h"
#include "../include/view/viewUtil.h"

void inputIntegerInRange(int* target, int min, int max, const char* prompt) {
    char buf[50];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;

        buf[strcspn(buf, "\n")] = '\0';

        if (!isIntegerBelongRange(buf, min, max)) {
            uiError("Please enter a valid integer only from %d to %d.\n", min,
                    max);
            continue;
        }
        *target = atoi(buf);
        return;
    }
}

void inputDoubleInRange(double* target, double min, double max,
                        const char* prompt) {
    char buf[50];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;

        buf[strcspn(buf, "\n")] = '\0';

        if (!isDoubleBelongRange(buf, min, max)) {
            uiError("Please enter a valid number only from %.2f to %.2f.\n", min,
                    max);
            continue;
        }
        *target = atof(buf);
        return;
    }
}

// Input yes no option
void inputYesNo(int* option, const char* prompt) {
    inputIntegerInRange(option, 0, 1, prompt);
}

// Input string
int inputString(char* buf, int size, const char* prompt) {
    printf("%s", prompt);

    if (!fgets(buf, size, stdin)) {
        return 0;
    }

    // Input too long
    if (!strchr(buf, '\n')) {
        while (getchar() != '\n');

        return 0;
    }

    buf[strcspn(buf, "\n")] = '\0';

    return 1;
}

// Input member name
void inputMemberName(char* target, const char* prompt) {
    char buf[50];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Please enter a valid name.\n");
            continue;
        }
        if (!isValidName(buf)) {
            uiError("Please enter a valid name.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Input  email
void inputMemberEmail(char* target, const char* prompt) {
    char buf[50];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Please enter a valid name.\n");
            continue;
        }
        if (!isValidEmail(buf)) {
            uiError("Please enter a valid email.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Input student ID
void inputStudentID(char* target, const char* prompt) {
    char buf[10];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Please enter a valid student ID.\n");
            continue;
        }

        if (!isValidStudentID(buf)) {
            uiError("Please enter a valid student ID.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Input new phone number
void inputMemberPhone(char* target, const char* prompt) {
    char buf[15];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Please enter a valid phone number.\n");
            continue;
        }

        if (!isValidPhone(buf)) {
            uiError("Please enter a valid phone number.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Input member role
void inputMemberRole(int* target, const char* prompt) {
    inputIntegerInRange(target, 0, 2, prompt);
}

// Input member team
void inputMemberTeam(int* target, const char* prompt) {
    inputIntegerInRange(target, 0, 3, prompt);
}

void inputPassword(char* target, const char* prompt) {
    char buf[30];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Please enter a valid password.\n");
            continue;
        }
        if (strlen(buf) < 6) {
            uiError("Password must be at least 6 characters long.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}


int inputPasswordOrCancel(char* target, const char* prompt) {
    char buf[30];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Please enter a valid password.\n");
            continue;
        }

        if (strcmp(buf, "q") == 0 || strcmp(buf, "quit") == 0 ||
            strcmp(buf, "back") == 0) {
            return 0;
        }

        if (strlen(buf) < 6) {
            uiError("Password must be at least 6 characters long. \nTry again: ");
            continue;
        }

        strcpy(target, buf);
        return 1;
    }
}

void inputValidTime(time_t* target, const char* prompt) {
    char buf[100];

    while (1) {
        printf("%s", prompt);

        if (!fgets(buf, sizeof(buf), stdin)) {
            continue;
        }

        buf[strcspn(buf, "\n")] = '\0';

        struct tm tm = {0};

        // basic value
        int year = 0;
        int month = 1;
        int day = 1;
        int hour = 0;
        int minute = 0;

        int matched = 0;

        matched = sscanf(buf, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute);

        if (matched < 5) {
            matched = sscanf(buf, "%d-%d-%d %d", &year, &month, &day, hour);
        }

        if (matched < 4) {
            matched = sscanf(buf, "%d-%d-%d", &year, &month, &day);
        }

        if (matched < 3) {
            matched = sscanf(buf, "%d-%d", &year, &month);
        }

        if (matched < 2) {
            matched = sscanf(buf, "%d", &year);
        }

        if (matched < 1) {
            uiError("Invalid format. Use YYYY-MM-DD HH:MM\n");
            continue;
        }

        if (!isValidDate(day, month, year) ||
            hour < 0 || hour > 23 ||
            minute < 0 || minute > 59) {
            uiError("Invalid date/time value.\n");
            continue;
        }

        tm.tm_year = year - 1900;
        tm.tm_mon  = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min  = minute;
        tm.tm_sec  = 0;

        *target = mktime(&tm);

        return;
    }
}

void inputTimeRange(time_t* start, time_t* end, const char* prompt) {

    while (1) {
        printf("%s%s%s\n", UI_TABLE_HEADER, prompt, UI_RESET);

        inputValidTime(
            start,
            "Enter start time (YYYY-MM-DD HH:MM): "
        );

        inputValidTime(
            end,
            "Enter end time (YYYY-MM-DD HH:MM): "
        );

        if (*start > *end) {
            uiError("Start time must be before end time.\n");
            continue;
        }

        return;
    }
}
