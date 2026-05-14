#ifndef MEMBER_H
#define MEMBER_H

#define MAX_MEMBERS 1000
#define MEMBERS_FILE "data/members.dat"

#include "../include/auth.h"
#include "../include/violation.h"
#include "model.h"

// file
int loadMembers(MemberList* members);
int saveMembers(const MemberList* members);

// Returns index in array, -1 if not found
int searchMemberByIdInM(const MemberList * members, const char* id);

// Recalculates totalFine from unpaid violations, returns 1 on success
int updateMemberTotalFine(MemberList* members, const ViolationList* violations, const char* id);

// Count unpaid violations for a member
int countUnpaidViolations(const char* id, const ViolationList* violations);

// Features 2.1: Add member
void addMember(MemberList* members, AccountList* accounts, const char *actorID);

// Features 2.2: Remove member
void removeMember(MemberList* members, AccountList* accounts, ViolationList* violations, const char *actorID);

// Features 2.3: Update member
void updateMember(MemberList* members, ViolationList* violations, const char *actorID);

int updateConsecutiveAbsences(MemberList* members, const char* id);

Member* getMemberById(const char* studentId, MemberList* members);

void removeOneMember (MemberList *members, AccountList *accounts, ViolationList *violations, const char *id, const char *actorID);

int checkTotalBOD(MemberList *members);

#endif
