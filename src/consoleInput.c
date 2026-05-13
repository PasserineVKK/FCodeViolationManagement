#include "../include/consoleInput.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/validate.h"
#include "../include/view/viewUtil.h"
#include "../include/model.h"

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
    char buf[50];

    while (1) {
        printf("%s", prompt);

        if (!fgets(buf, sizeof(buf), stdin)) {
            continue;
        }

        buf[strcspn(buf, "\n")] = '\0';

        struct tm tm = {0};

        int year, month, day, hour, minute;

        if (sscanf(buf,
                   "%d-%d-%d %d:%d",
                   &year,
                   &month,
                   &day,
                   &hour,
                   &minute) != 5) {

           uiError("Please enter a valid time in format YYYY-MM-DD HH:MM.\n");
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

void getReason(int *reason)
{
    printf("Choose violate reason: \n"); // Fixed redundant format arg
    printf("%d - Not uniform\n", REASON_NOT_UNIFORM);
    printf("%d - Meeting absence\n", REASON_MEETING_ABSENCE);
    printf("%d - Not join in Club activity\n", REASON_NO_CLUB_ACTIVITY);
    printf("%d - Violence\n", REASON_VIOLENCE);
    inputIntegerInRange(reason, 0, 3, "Please enter: ");
}
