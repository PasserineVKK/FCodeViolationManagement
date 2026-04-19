#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "fileio.h"

//file
int loadAccounts(Account accounts[], int *count) {
    return loadFromFile(ACCOUNTS_FILE, accounts, sizeof(Account), MAX_ACCOUNTS, count);
}

int saveAccounts(Account accounts[], int count) {
    return saveToFile(ACCOUNTS_FILE, accounts, sizeof(Account), count);
}
