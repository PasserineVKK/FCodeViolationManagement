#ifndef ACCOUNT_H
#define ACCOUNT_H

typedef struct
{
	char studentID[8];
	char password[30];
	int role;	   // 0 = Member, 1 = BOD
	int isLocked;  // 1 = This account locked after 3 failed trials
	int failCount; // consecutive failed trials
} Account;

#endif