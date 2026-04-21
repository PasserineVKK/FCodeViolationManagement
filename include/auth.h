#ifndef ACCOUNT_H
#define ACCOUNT_H
#define MAX_ACCOUNTS 1000
#define ACCOUNTS_FILE "data/accounts.dat"

typedef struct
{
	char studentID[10]; // SE000000\0
	char password[30];
	int role;	   // 0 = Member, 1 = BOD
	int isLocked;  // 1 = This account locked after 3 failed trials
	int failCount; // consecutive failed trials
} Account;
// file
int loadAccounts(Account accounts[], int *count);
int saveAccounts(Account accounts[], int count);



#endif