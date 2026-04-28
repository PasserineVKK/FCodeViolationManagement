#include "../include/auth.h"

#include <stdio.h>
#include <string.h>

#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"

// file
int loadAccounts(Account accounts[], int* count) {
    return loadFromFile(ACCOUNTS_FILE, accounts, sizeof(Account), MAX_ACCOUNTS,
                        count);
}

int saveAccounts(Account accounts[], int count) {
    return saveToFile(ACCOUNTS_FILE, accounts, sizeof(Account), count);
}

int searchMemberByIdInA(Account accounts[], int count, const char* id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(accounts[i].studentID, id) == 0) {
            return i;
        }
    }
    return -1;
}

static int inputPasswordOrCancel(char* target, const char* prompt) {
    char buf[30];
    while (1) {
        if (!inputString(buf, sizeof(buf), prompt)) {
            printf("Please enter a valid password.\n");
            continue;
        }

        if (strcmp(buf, "q") == 0 || strcmp(buf, "quit") == 0 ||
            strcmp(buf, "back") == 0) {
            return 0;
        }

        if (strlen(buf) < 6) {
            printf("Password must be at least 6 characters long.\n");
            continue;
        }

        strcpy(target, buf);
        return 1;
    }
}

// Return role of logged in account
int login(Account accounts[], char* studentID, int aCount) {
    printf("===== LOGIN =====\n");

    // char studentID[9]; // SE000000\0
    char password[30];
    int role;           // 0 = Member, 1 = BOD
    int isLocked = 0;   // 1 = This account locked after 3 failed trials
    int failCount = 0;  // consecutive failed trials

    // Input student ID
    inputStudentID(studentID, "Enter student ID: ");
    int aIndex = searchMemberByIdInA(accounts, aCount, studentID);

    // Check if student ID exists in accounts list
    if (aIndex == -1) {
        printf("\nStudent ID not found. Please try again.\n");
        return -2;
    }

    // Check if account is locked
    if (accounts[aIndex].isLocked) {
        printf("\nThis account is locked due to 3 failed login attempts.\n");
        return -1;
    }

    // Enter password
    do {
        printf("Enter password: ");
        scanf("%s", password);
        // clear stadin
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        if (strcmp(password, accounts[aIndex].password) == 0) {
            break;
        }
        printf("Incorrect password. Please try again.\n");
        failCount++;
    } while (failCount < 3);

    // Check if account is locked after 3 failed attempts
    if (failCount >= 3) {
        accounts[aIndex].isLocked = 1;
        saveAccounts(accounts, aCount);
        printf("\nThis account is now locked due to 3 failed login attempts.\n");
        return -1;
    }

    // Successful login, return role
    return accounts[aIndex].role;
}

// Change password of logged in account
void changePassword(Account accounts[], int aCount, char* actorID, int role) {
    printf("===== CHANGE PASSWORD =====\n");
    int aIndex = -1;

    char studentID[9];  // SE000000\0

    char oldPassword[30];

    if (role == 0) {
        strcpy(studentID, actorID);
    } else {
        // Input student ID which want to change password
        inputStudentID(
            studentID,
            "Enter student ID of the member you want to change password: ");
    }

    aIndex = searchMemberByIdInA(accounts, aCount, studentID);

    if (aIndex == -1) {
        printf("Student ID not found. Please try again.\n");
        return;
    }

    // Display student ID which want to change password
    printf("Student ID: %s\n", studentID);

    // if actor is normal member OR actor is changing his/her own pass
    if (role == 0 || strcmp(actorID, studentID) == 0) {
        // Enter old password
        do {
            printf("Enter old password (type q, quit, or back to cancel): ");
            if (!inputPasswordOrCancel(oldPassword, "")) {
                printf("Password change cancelled.\n");
                return;
            }

            // Check if old password is correct
            if (strcmp(oldPassword, accounts[aIndex].password) == 0) {
                break;
            }
            printf("Incorrect old password. Please try again.\n");
        } while (1);
    }

    char newPassword[30];
    do {
        // Enter new password
        inputPassword(newPassword, "Enter new password: ");
        char confirmPassword[30];
        inputPassword(confirmPassword, "Confirm new password: ");

        // Check if new password and confirm password match
        if (strcmp(newPassword, confirmPassword) == 0) {
            break;
        }

        printf("New password and confirm password do not match.\n");
    } while (1);

    // Update new password to account and save to file
    strcpy(accounts[aIndex].password, newPassword);
    saveAccounts(accounts, aCount);
    printf("Password changed successfully.\n");

    return;
}
