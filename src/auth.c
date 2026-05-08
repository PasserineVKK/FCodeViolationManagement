#include "../include/auth.h"

#include <stdio.h>
#include <string.h>

#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"

// file
int loadAccounts(AccountList *accounts) {
    return loadFromFile(ACCOUNTS_FILE, accounts->data, sizeof(Account), MAX_ACCOUNTS,
                        &accounts->count);
}

// 
int saveAccounts(AccountList *accounts) {
    return saveToFile(ACCOUNTS_FILE, accounts->data, sizeof(Account), accounts->count);
}

// use struct pointer as param to avoid shallow copy
int searchMemberByIdInA(AccountList *accounts, const char* id) {
    for (int i = 0; i < accounts->count; i++) {
        if (strcmp(accounts->data[i].studentID, id) == 0) {
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
int login(AccountList *accounts, char* studentID) {
    printf("===== LOGIN =====\n");

    char password[30];
    
    // Input student ID
    inputStudentID(studentID, "Enter student ID: ");
    
    int aIndex = searchMemberByIdInA(accounts, studentID);
    
    // Check if student ID exists in accounts list
    if (aIndex == -1) {
        printf("\nStudent ID not found. Please try again.\n");
        return -2;
    }

    Account acc = accounts->data[aIndex];

    // Check if account is locked
    if (acc.isLocked) {
        printf("\nThis account is locked due to 3 failed login attempts.\n");
        return -1;
    }

    int failCount = acc.failCount;  // consecutive failed trials
    
    // Enter password
    while (failCount < 3) {
        printf("Enter password: ");
        scanf("%s", password);
        // clear stadin
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        if (strcmp(password, acc.password) == 0) {
            break;
        }
        printf("Incorrect password. Please try again.\n");
        failCount++;
        accounts->data[aIndex].failCount = failCount;
        saveAccounts(accounts); // Truy?n con tr?
    };

    // Check if account is locked after 3 failed attempts
    if (failCount >= 3) {
        accounts->data[aIndex].isLocked = 1;
        saveAccounts(accounts);
        printf("\nThis account is now locked due to 3 failed login attempts.\n");
        return -1;
    }

    // Successful login, reset fail, return role
    accounts->data[aIndex].failCount = 0; 
    saveAccounts(accounts);
    return accounts->data[aIndex].role;
}

// Change password of logged in account
void changePassword(AccountList *accounts, char* actorID, int role) {
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

    // Truy?n con tr? accounts
    aIndex = searchMemberByIdInA(accounts, studentID);

    if (aIndex == -1) {
        printf("Student ID not found. Please try again.\n");
        return;
    }

    // Display student ID which want to change password
    printf("Student ID: %s\n", studentID);

    int targetRole = accounts->data[aIndex].role;

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
            if (strcmp(oldPassword, accounts->data[aIndex].password) == 0) {
                break;
            }
            printf("Incorrect old password. Please try again.\n");
        } while (1);
    } else if (targetRole == 2) {
        printf("You are not granted permission to change BOD password.\n");
        return;
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

    // Update new password to account
    strcpy(accounts->data[aIndex].password, newPassword);
    saveAccounts(accounts);
    printf("Password changed successfully.\n");
 
    return;
}
