#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "model.h"

#define MAX_ACCOUNTS 1000
#define ACCOUNTS_FILE "data/accounts.dat"

#define LOCKED_ACC 1
#define NOT_LOCKED_ACC 0

int loadAccounts(AccountList *accounts);
int saveAccounts(AccountList *accounts);

int searchMemberByIdInA(AccountList *accounts, const char* id);

// Gets the role of the current user.
int login(AccountList *accounts, char *studentID);

// Change password of logged in account
void changePassword(AccountList *accounts, char* actorID, int actorRole);

long getRemainingLockTime(Account *acc);

void handleLockedAccount(AccountList* accounts);
#endif
