#include <stdio.h>
#include <string.h>
#include "../include/auth.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/consoleInput.h"

//file
int loadAccounts(Account accounts[], int *count) {
    return loadFromFile(ACCOUNTS_FILE, accounts, sizeof(Account), MAX_ACCOUNTS, count);
}

int saveAccounts(Account accounts[], int count) {
    return saveToFile(ACCOUNTS_FILE, accounts, sizeof(Account), count);
}

int searchMemberByIdInA(Account accounts[], int count, const char *id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(accounts[i].studentID, id) == 0) {
            return i;
        }
    }
    return -1;
}

//Return role of logged in account
int login(Account accounts[], int aCount){

    printf("===== LOGIN =====\n");

	char studentID[9]; // SE000000\0
	char password[30];
	int role;	   // 0 = Member, 1 = BOD
	int isLocked;  // 1 = This account locked after 3 failed trials
	int failCount; // consecutive failed trials

    //Input student ID
    inputStudentID(studentID, "Enter student ID: ");
    int aIndex = searchMemberByIdInA(accounts, aCount, studentID);
    
    //Check if student ID exists in accounts list
    if (aIndex == -1) {
        printf("Student ID not found. Please try again.\n");
        return -1;
    }

    //Check if account is locked
    if (accounts[aIndex].isLocked) {
        printf("This account is locked due to 3 failed login attempts.\n");
        return -1;
    }

    //Enter password 
    do {
        printf("Enter password: ");
        scanf("%s", password);
        if (strcmp(password, accounts[aIndex].password) != 0) {
            printf("Incorrect password. Please try again.\n");
            failCount++;
        }
    } while (failCount < 3);

    //Check if account is locked after 3 failed attempts
    if (failCount >= 3) {
        accounts[aIndex].isLocked = 1;
        saveAccounts(accounts, aCount);
        printf("This account is now locked due to 3 failed login attempts.\n");
        return -1;
    }

    // Successful login, return role
    return accounts[aIndex].role;
}

//Logout by go to login screen 
void logout();

//Change password of logged in account
void changePassword(Account accounts[], int aCount){
    char studentID[9]; // SE000000\0

    char oldPassword[30];

    //Input student ID which want to change password
    inputStudentID(studentID, "Enter student ID: ");
    int aIndex = searchMemberByIdInA(accounts, aCount, studentID);

    //Enter old password
    do {
        printf("Enter old password: ");
        scanf("%s", oldPassword);
        
        //Check if old password is correct
        if (strcmp(oldPassword, accounts[aIndex].password) == 0) {
            break;
        }

        printf("Incorrect old password. Please try again.\n");
    } while (1);

    //Enter new password
    char newPassword[30];
    printf("Enter new password: ");
    scanf("%s", newPassword);

    //Update new password to account and save to file
    strcpy(accounts[aIndex].password, newPassword);
    saveAccounts(accounts, aCount);
    printf("Password changed successfully.\n");
    
    return;
}

