#include "../include/auth.h"

#include <stdio.h>
#include <string.h>

#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/view/viewUtil.h"
// file
int loadAccounts(AccountList *accounts)
{
    return loadFromFile(ACCOUNTS_FILE, accounts->data, sizeof(Account), MAX_ACCOUNTS,
                        &accounts->count);
}

//
int saveAccounts(AccountList *accounts)
{
    return saveToFile(ACCOUNTS_FILE, accounts->data, sizeof(Account), accounts->count);
}

// use struct pointer as param to avoid shallow copy
int searchMemberByIdInA(AccountList *accounts, const char *id)
{
    for (int i = 0; i < accounts->count; i++)
    {
        if (strcmp(accounts->data[i].studentID, id) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Return role of logged in account
int login(AccountList *accounts, char *studentID)
{
    uiInfo("===LOGIN===\n");

    char password[30];

    // Input student ID
    inputStudentID(studentID, "Enter student ID: ");

    int aIndex = searchMemberByIdInA(accounts, studentID);


    // Check if student ID exists in accounts list
    if (aIndex == -1)
    {
        printf("\nStudent ID not found. Please try again.\n");
        return -2;
    }

    Account *acc = &accounts->data[aIndex];

    // Check if account is locked
    if (acc->isLocked || acc->failCount >= 9)
    {
        acc->isLocked = 1;
        saveAccounts(accounts);
        uiError("\nAccount is permanently locked. Please contact Admin.\n");
        return -1;
    }

    long remaining = getRemainingLockTime(acc);
    if (remaining > 0)
    {
        int turnback = 0;
        printf("\nAccount is temporarily locked.\n");
        inputYesNo(&turnback, "Do you want to wait for countdown (Enter 0), or Turn Back (Enter 1): ");
        if (turnback)
            return -1;
        while (getRemainingLockTime(acc) > 1)
        {
            displayCountdown(getRemainingLockTime(acc));
        }
    }

    // Enter password
    inputString(password, sizeof(password), "Enter password: ");

    if (strcmp(password, acc->password) == 0)
    {
        // Success
        acc->failCount = 0;
        acc->lockedFrom = 0;
        saveAccounts(accounts);
        return acc->role;
    }
    else
    {
        // Wrong password
        acc->failCount++;
        acc->lockedFrom = time(NULL); // Last failed time
        saveAccounts(accounts);
        if (acc->failCount >= 9)
        {
            acc->isLocked = 1;
            uiError("\nToo many failed attempts. Account is now PERMANENTLY locked.\n");
            return -1;
        }
        else if (acc->failCount % 3 == 0)
        {
            // if 3 wait 5 mins, if 6 wait 60 mins
            long wait = (acc->failCount == 3) ? 5 : 60;
            printf("\nIncorrect password. Account locked for %ld minutes.\n", wait);
            int turnback = 0;
            inputYesNo(&turnback, "\nDo you want to wait for countdown (Enter 0), or Turn Back (Enter 1): ");
            if (turnback)
                return -1;
            while (getRemainingLockTime(acc) > 1)
            {
                displayCountdown(getRemainingLockTime(acc));
            }
        }
        else
        {
            printf("\nIncorrect password. (Attempt % d/9)\n", acc->failCount);
        }
        return -1;
    }
}

// Change password of logged in account
void changePassword(AccountList *accounts, char *actorID, int role)
{
    uiInfo("CHANGE PASSWORD\n");

    int aIndex = -1;

    char studentID[9]; // SE000000\0
    char oldPassword[30];

    if (role == 0)
    {
        strcpy(studentID, actorID);
    }
    else
    {
        // Input student ID which want to change password
        inputString(studentID, sizeof(studentID), "Enter student ID of the member you want to change password: ");
    }

    aIndex = searchMemberByIdInA(accounts, studentID);

    if (aIndex == -1)
    {
        printf("Student ID not found. Please try again.\n");
        return;
    }

    // Display student ID which want to change password
    printf("Student ID: %s\n", studentID);

    int targetRole = accounts->data[aIndex].role;

    // if actor is normal member OR actor is changing his/her own pass
    if (role == 0 || strcmp(actorID, studentID) == 0)
    {
        // Enter old password
        if (inputPasswordOrCancel(oldPassword, "Enter old password (Press q, quit, or back to cancel)"))
        {
            printf("Password change cancelled");
            return;
        }
    }
    else if (role < 2 && targetRole == 2)
    {
        printf("You are not granted permission to change BOD password.\n");
        return;
    }

    char newPassword[30];
    do
    {
        // Enter new password
        inputPassword(newPassword, "Enter new password: ");
        char confirmPassword[30];
        inputPassword(confirmPassword, "Confirm new password: ");

        // Check if new password and confirm password match
        if (strcmp(newPassword, confirmPassword) == 0)
        {
            break;
        }

        printf("New password and confirm password do not match.\n");
    } while (1);

    // Update new password to account
    strcpy(accounts->data[aIndex].password, newPassword);
    saveAccounts(accounts);
    uiSuccess("Password changed successfully.\n");

    return;
}

#include <time.h>
long getRemainingLockTime(Account *acc)
{
    if (acc->isLocked)
        return -1;
    if (acc->failCount == 0 || acc->failCount % 3 != 0)
        return 0;

    time_t now = time(NULL);
    double diff = difftime(now, acc->lockedFrom);
    long lockDuration = 0;

    if (acc->failCount == 3)
        lockDuration = 300; // lock in 5 min
    else if (acc->failCount == 6)
        lockDuration = 3600; // lock in 1 hour
    else if (acc->failCount >= 9)
        return -1; // Require admin to unlock

    if (diff < lockDuration)
    {
        return lockDuration - (long)diff;
    }
    return 0;
}

void handleLockedAccount(AccountList *accounts)
{
    if (accounts == NULL)
    {
        uiError("Can not open accounts data.");
        return;
    }
    char studentID[10];
    inputStudentID(studentID, "Input student ID of account: ");
    int aIndex = searchMemberByIdInA(accounts, studentID);
    if (aIndex != -1)
    {
        accounts->data[aIndex].isLocked = 0;
        accounts->data[aIndex].failCount = 0;
        saveAccounts(accounts);
        uiSuccess("unlock Successfully");
    }
    else
    {
        printf("Can not find corresponding account for %s", studentID);
        return;
    }
}
