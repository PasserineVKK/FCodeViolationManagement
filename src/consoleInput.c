#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/validate.h"

// 1.1. Input integer
void inputInteger (int *target, const char * prompt){
	char buf[50];

    while (1) {
        printf("%s", prompt);

        if (!fgets(buf, sizeof(buf), stdin))
            continue;

        buf[strcspn(buf, "\n")] = '\0';

        if (!isPosInteger(buf)) {
            printf("? Please enter a valid integer.\n");
            continue;
        }

        *target = atoi(buf);
        return;
    }
}

// 1.2. Input real number
void inputPosFloat(float *target, const char *prompt) {
    char buf[50];

    while (1) {
        printf("%s", prompt);

        if (!fgets(buf, sizeof(buf), stdin))
            continue;

        buf[strcspn(buf, "\n")] = '\0';

        if (!isPosFloat(buf)) {
            printf("? Please enter a valid positive float.\n");
            continue;
        }

        *target = atof(buf);
        return;
    }
}

// 1.3 Input yes no option
void inputYesNo (int *option, const char * prompt){
    char buf[10];

    while (1) {
        printf("%s", prompt);

        if (!fgets(buf, sizeof(buf), stdin))
            continue;

        buf[strcspn(buf, "\n")] = '\0';

        if (!isOneOrZero(buf)) {
            printf("? Please enter a valid yes/no option (0 or 1).\n");
            continue;
        }

        *option = atoi(buf);
        return;
    }
}

// 1.4 Input one or zero
void inputStudentID (char *target, const char * prompt){
    char buf[9];

    while (1) {
        printf("%s", prompt);

        if (!fgets(buf, sizeof(buf), stdin))
            continue;

        buf[strcspn(buf, "\n")] = '\0';

        if (!isValidMemberID(buf)) {
            printf("?Please enter a valid student ID.\n");
            continue;
        }

        strcpy(target, buf);
        return;
    }
}
