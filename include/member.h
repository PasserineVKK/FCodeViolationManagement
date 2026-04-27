#ifndef MEMBER_H
#define MEMBER_H

#define MAX_MEMBERS 1000
#define MEMBERS_FILE "data/members.dat"

#include "../include/violation.h"
#include "../include/auth.h"

#include "model.h"

// file
int loadMembers(Member members[], int *count);
int saveMembers(Member members[], int count);

// Returns index in array, -1 if not found
int searchMemberByIdInM(Member members[], int count, const char *id);

// Recalculates totalFine from unpaid violations, returns 1 on success
int updateMemberTotalFine(Member members[], int mCount, Violation violations[], int vCount, const char *id);

// Count unpaid violations for a member
int countUnpaidViolations(const char *id, Violation violations[], int vCount);

// Features 2.1: Add member
void addMember(Member members[], int *mCount, Account accounts[], int *aCount);

// Features 2.2: Remove member
void removeMember(Member members[], int *mCount, Account accounts[], int *aCount, Violation violations[], int *vCount);

// Features 2.3: Update member
void updateMember(Member members[], int *mCount, Violation violations[], int vCount);

// Features 2.7: Show member list
void showTotalFineByRole(Member members[], int count);

int updateConsecutiveAbsences(Member members[], int count, const char *id);

Member *getMemberById(const char *studentId, Member members[], int mCount);

#endif
