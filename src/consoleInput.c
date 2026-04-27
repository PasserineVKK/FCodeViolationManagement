#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/validate.h"

void inputIntegerInRage (int *target, int min, int max, const char * prompt){
    char buf[50];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin))
            continue;
        
        buf[strcspn(buf, "\n")] = '\0';

        if (!isIntegerBelongRange(buf, min, max)) {
            printf("Please enter a valid integer only from %d to %d.\n", min, max);
            continue;
        }
        *target = atoi(buf);
        return;
    }
}

void inputDoubleInRange (double *target, double min, double max, const char * prompt){
    char buf[50];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin))
            continue;
        
        buf[strcspn(buf, "\n")] = '\0';

        if (!isDoubleBelongRange(buf, min, max)) {
            printf("Please enter a valid number only from %.2f to %.2f.\n", min, max);
            continue;
        }
        *target = atof(buf);
        return;
    }
}

// Input yes no option
void inputYesNo (int *option, const char * prompt){
    inputIntegerInRage (option, 0, 1, prompt);
}

//Input string 
int inputString(char *buf, int size, const char *prompt)
{
    printf("%s", prompt);

    if (!fgets(buf, size, stdin)){
        return 0;
    }

    // Input too long
    if (!strchr(buf, '\n')){
        while (getchar() != '\n');

        return 0;
    }

    buf[strcspn(buf, "\n")] = '\0';

    return 1;
}

// Input member name
void inputMemberName (char *target, const char * prompt){
    char buf[50];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            printf("Please enter a valid name.\n");
            continue;
        }
        if (!isValidName(buf)) {
            printf("Please enter a valid name.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Input  email
void inputMemberEmail (char *target, const char * prompt){
    char buf[50];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            printf("Please enter a valid name.\n");
            continue;
        }
        if (!isValidEmail(buf)) {
            printf("Please enter a valid email.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Input student ID
void inputStudentID (char *target, const char * prompt){
    char buf[10];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            printf("Please enter a valid student ID.\n");
            continue;
        }

        if (!isValidStudentID(buf)) {
            printf("Please enter a valid student ID.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Input new phone number
void inputMemberPhone (char *target, const char * prompt){
    char buf[15];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            printf("Please enter a valid phone number.\n");
            continue;
        }

        if (!isValidPhone(buf)) {
            printf("Please enter a valid phone number.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// Input member role
void inputMemberRole (int *target, const char * prompt){
    inputIntegerInRage (target, 0, 2, prompt);
}

// Input member team
void inputMemberTeam (int *target, const char * prompt){
    inputIntegerInRage (target, 0, 3, prompt);
}


// Input password
void inputPassword (char *target, const char * prompt){
    char buf[30];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            printf("Please enter a valid password.\n");
            continue;
        }
        if (strlen(buf) < 6) {
            printf("Password must be at least 6 characters long.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}