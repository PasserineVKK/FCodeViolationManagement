#ifndef ACCOUNT_H
#define ACCOUNT_H
#define MAX_ACCOUNTS 1000
#define ACCOUNTS_FILE "data/accounts.dat"

typedef struct
{
	char studentID[9]; // SE000000\0
	char password[30];
	int role;	   // 0 = Member, 1 = BOD
	int isLocked;  // 1 = This account locked after 3 failed trials
	int failCount; // consecutive failed trials
} Account;
// file
int loadAccounts(Account accounts[], int *count);
int saveAccounts(Account accounts[], int count);

int searchMemberByIdInA(Account accounts[], int count, const char *id);


//Return role of logged in account
int login(Account accounts[], char* studentID, int aCount);

//Logout by go to login screen 
void logout();

//Change password of logged in account
void changePassword(Account accounts[], int aCount);

#endif
