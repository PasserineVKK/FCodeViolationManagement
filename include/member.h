#ifndef MEMBER_H
#define MEMBER_H
#define MAX_MEMBERS 1000
#define MEMBERS_FILE "data/members.dat"

#include "violation.h"
typedef struct
{
	char fullName[100];
	char email[100];
	char phoneNumber[11];
	char studentID[10]; // SE000000\0
	int team;			// 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
	int role;			// 0 = Member, 1 = Leader/Vice, 2 = BOD
	int violationCount;
	int consecutiveAbsences;
	double totalFine;
	int status;
	int isPending; // 0 = not pending, 1 = pending
} Member;

// file
int loadMembers(Member members[], int *count);
int saveMembers(Member members[], int count);

// Returns index in array, -1 if not found
int searchMemberById(Member members[], int count, const char *id);

// Recalculates totalFine from unpaid violations, returns 1 on success
int updateMemberTotalFine(const char *id);

#endif