#ifndef ACCOUNT_H
#define ACCOUNT_H
#define MAX_ACCOUNTS 1000
#define ACCOUNTS_FILE "data/accounts.dat"

#include "model.h"

// file
int loadAccounts(Account accounts[], int *count);
int saveAccounts(Account accounts[], int count);

#endif