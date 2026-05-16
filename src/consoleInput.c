#include "../include/consoleInput.h"

#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/validate.h"
#include "../include/view/viewUtil.h"
#include "../include/model.h"

// Prompt the user for an integer and enforce it lies within [min, max].
// Re-prompts until a valid integer in range is entered and stores it in `target`.

void inputIntegerInRange(int* target, int min, int max, const char* prompt) {
    char buf[50];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;

        buf[strcspn(buf, "\n")] = '\0';

        if (!isIntegerBelongRange(buf, min, max)) {
                uiError("Enter a number from %d to %d.\n", min,
                    max);
            continue;
        }
        *target = atoi(buf);
        return;
    }
}

// Prompt the user for a double and enforce it lies within [min, max].
// Re-prompts until a valid double in range is entered and stores it in `target`.
void inputDoubleInRange(double* target, double min, double max,
                        const char* prompt) {
    char buf[50];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;

        buf[strcspn(buf, "\n")] = '\0';

        if (!isDoubleBelongRange(buf, min, max)) {
                uiError("Enter a number from %.2f to %.2f.\n", min,
                    max);
            continue;
        }
        *target = atof(buf);
        return;
    }
}

// Prompt for a yes/no selection (0 or 1). Stores the result in `option`.
void inputYesNo(int* option, const char* prompt) {
    inputIntegerInRange(option, 0, 1, prompt);
}

// Reads a line of text into `buf` up to `size - 1` characters.
// Returns 1 on success, 0 if input was too long or failed.
int inputString(char* buf, int size, const char* prompt) {
    printf("%s", prompt);

    if (!fgets(buf, size, stdin)) {
        printf("11111");
        return 0;
    }

    // Input too long
    if (!strchr(buf, '\n')) {
        printf ("2222");
        while (getchar() != '\n');

        return 0;
    }

    buf[strcspn(buf, "\n")] = '\0';

    return 1;
}

// Reads and validates a member full name, repeating until valid.
// The validated name is copied into `target`.
void inputMemberName(char* target, const char* prompt) {
    char buf[50];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Enter a valid name.\n");
            continue;
        }
        if (!isValidName(buf)) {
            uiError("Enter a valid name.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Reads and validates an email address, repeating until valid.
// The validated email is copied into `target`.
void inputMemberEmail(char* target, const char* prompt) {
    char buf[50];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Enter a valid email.\n");
            continue;
        }
        if (!isValidEmail(buf)) {
            uiError("Enter a valid email.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Reads and validates a student ID (format checked by `isValidStudentID`).
// The validated ID is copied into `target`.
void inputStudentID(char* target, const char* prompt) {
    char buf[10];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Enter a valid student ID.\n");
            continue;
        }

        if (!isValidStudentID(buf)) {
            uiError("Enter a valid student ID.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Reads and validates a phone number, repeating until valid.
// The validated phone number is copied into `target`.
void inputMemberPhone(char* target, const char* prompt) {
    char buf[15];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            uiError("Enter a valid phone number.\n");
            continue;
        }

        if (!isValidPhone(buf)) {
            uiError("Enter a valid phone number.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Prompts for a member role integer (0..2) and stores into `target`.
void inputMemberRole(int* target, const char* prompt) {
    inputIntegerInRange(target, 0, 2, prompt);
}

// Prompts for a member team integer (0..3) and stores into `target`.
void inputMemberTeam(int* target, const char* prompt) {
    inputIntegerInRange(target, 0, 3, prompt);
}

// Reads a password or allows the user to cancel by typing 'q'.
// Returns 1 when a password was provided, 0 when the user canceled.
int inputPasswordOrCancel(char* password, int len, const char* prompt){
    int i = 0;
    char ch;
    char buf[30];

    printf("%s", prompt);

    while (1)
    {
        // For no echo
        ch = _getch();

        // Finish -> enter
        if (ch == '\r' || ch == '\n')
        {
            if (i < 6) {
                uiError("\nPassword must be at least 6 characters long.\n");
                // Reset buffer
                i = 0;
                printf("%s", prompt);
                continue;
            }

            // Add end to buffer
            buf[i] = '\0';
            printf("\n");

            // Copy kết quả
            strcpy(password, buf);
            return 1;
        }

        //To quit input
        if (ch == 'q'){
            return 0;
        }

        // Delete
        if (ch == '\b'){
            if (i > 0){
                i--;
                printf("\b \b"); // back->add black-> back again 
            }
        }
        //Add to buff
        else if (i < len - 1){
            buf[i++] = ch;

            // In *
            printf("*");
        }
    }
}



// Parses a date/time string and converts it into a time_t stored in `target`.
// Accepts several formats (YYYY, YYYY-MM, YYYY-MM-DD, YYYY-MM-DD HH, YYYY-MM-DD HH:MM).
// Keeps prompting until a valid past/future date/time is entered according to `isValidDate`.
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

// Prompts the user for a start and end time using `inputValidTime` and validates the range.
// Stores results in `start` and `end`.
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
