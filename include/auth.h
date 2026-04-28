#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "model.h"

#define MAX_ACCOUNTS 1000
#define ACCOUNTS_FILE "data/accounts.dat"

int loadAccounts(Account accounts[], int *count);
int saveAccounts(Account accounts[], int count);

int searchMemberByIdInA(Account accounts[], int count, const char *id);

// Return role of logged in account
int login(Account accounts[], char *studentID, int aCount);

// Change password of logged in account
void changePassword(Account accounts[], int aCount, char *actorID, int role);

#endif
