#ifndef MEMBER_H
#define MEMBER_H

#define MAX_MEMBERS 1000
#define MEMBERS_FILE "data/members.dat"

#include "../include/violation.h"
#include "../include/auth.h"



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
	int isPending; //0 = not pending, 1 = pending
} Member;

// file
int loadMembers(Member members[], int *count);
int saveMembers(Member members[], int count);

// Returns index in array, -1 if not found
int searchMemberByIdInM(Member members[], int count, const char *id);

// Recalculates totalFine from unpaid violations, returns 1 on success
int updateMemberTotalFine(Member members[], int mCount, Violation violations[], int vCount, const char *id);

// Display member info in table format
void displayOneMemberInfo (Member member);

// Count unpaid violations for a member
int  countUnpaidViolations(const char *id, Violation violations[], int vCount);

// Features 2.1: Add member
void addMember(Member members[], int *mCount, Account accounts[], int aCount);

// Features 2.2: Remove member
void removeMember(Member members[], int *mCount, Account accounts[], int *aCount, Violation violations[], int *vCount);

// Features 2.3: Update member
void updateMember(Member members[], int *mCount, Violation violations[], int vCount);

// Features 2.7: Show member list
void showTotalFineByRole(Member members[], int count);



int updateConsecutiveAbsences(Member members[], int count, const char *id);

#endif
