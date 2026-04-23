#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/validate.h"

// ===== I. INPUT INT, FLOAT, YES/NO OPTION ================================
// 1.1. Input integer
void inputPosInteger (int *target, const char * prompt){
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
// ===== II. INPUT MEMBER INFO ================================
// 2.1 Input member name
void inputMemberName (char *target, const char * prompt){
    char buf[50];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin))
            continue;
        // Check '\n', if don't have, string longer than buffer can get => fail
        if (!strchr(buf, '\n'))
            //Read all until '\n' to make buffer clean for next input
            while (getchar() != '\n');    
        buf[strcspn(buf, "\n")] = '\0';
        if (!isValidName(buf)) {
            printf("? Please enter a valid name.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

//2.2 Input  email
void inputMemberEmail (char *target, const char * prompt){
    char buf[50];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin))
            continue;
        // Check '\n', if don't have, string longer than buffer can get => fail
        if (!strchr(buf, '\n'))
            //Read all until '\n' to make buffer clean for next input
            while (getchar() != '\n');    
        buf[strcspn(buf, "\n")] = '\0';
        if (!isValidEmail(buf)) {
            printf("? Please enter a valid email.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// 2.3 Input student ID
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

//2.4 Input new phone number
void inputMemberPhone (char *target, const char * prompt){
    char buf[11];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin))
            continue;
        // Check '\n', if don't have, string longer than buffer can get => fail
        if (!strchr(buf, '\n'))
            //Read all until '\n' to make buffer clean for next input
            while (getchar() != '\n');    
        buf[strcspn(buf, "\n")] = '\0';
        if (!isValidPhone(buf)) {
            printf("? Please enter a valid phone number.\n");
            continue;
        }
        strcpy(target, buf);
        return;
    }
}

// 2.5 Input member role
void inputMemberRole (int *target, const char * prompt){
    char buf[2];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin))
            continue;
        buf[strcspn(buf, "\n")] = '\0';
        if (!isPosInteger(buf)) {
            printf("? Please enter a valid integer.\n");
            continue;
        }
        if (!isValidRole(buf)) {
            printf("? Please enter a valid role (0-2).\n");
            continue;
        }
        *target = atoi(buf);
        return;
    }
}

//2.6 Input member team
void inputMemberTeam (int *target, const char * prompt){
    char buf[2];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin))
            continue;
        buf[strcspn(buf, "\n")] = '\0';
        if (!isPosInteger(buf)) {
            printf("? Please enter a valid integer.\n");
            continue;
        }
        if (!isValidTeam(buf)) {
            printf("? Please enter a valid team (0-3).\n");
            continue;
        }
        *target = atoi(buf);
        return;
    }
}

// =========================================================================
