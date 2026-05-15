#ifndef MEMBER_H
#define MEMBER_H

#define MAX_MEMBERS 1000
#define MEMBERS_FILE "data/members.dat"

#include "../include/auth.h"
#include "../include/violation.h"
#include "model.h"

// -------- File I/O --------
int loadMembers(MemberList* members);
int saveMembers(const MemberList* members);

// -------- Accessors / Search --------
// Returns the array index, or -1 if not found.
int searchMemberByIdInM(const MemberList * members, const char* id);
Member* getMemberById(const char* studentId, MemberList* members);

// -------- Calculations / Summaries --------
// Recalculates totalFine from unpaid violations; returns 1 on success.
int updateMemberTotalFine(MemberList* members, const ViolationList* violations, const char* id);
// Counts unpaid violations for a member.
int countUnpaidViolations(const char* id, const ViolationList* violations);

// -------- Business utilities --------
int updateConsecutiveAbsences(MemberList* members, const char* id);
void removeOneMember (MemberList *members, AccountList *accounts, ViolationList *violations, const char *id, const char *actorID);
int checkTotalBOD(MemberList *members);

// -------- CRUD / Features --------
// Feature 2.1: Adds a member.
void addMember(MemberList* members, AccountList* accounts, const char *actorID);
// Feature 2.2: Removes a member.
void removeMember(MemberList* members, AccountList* accounts, ViolationList* violations, const char *actorID);
// Feature 2.3: Updates a member.
void updateMember(AccountList* accounts, MemberList* members, ViolationList* violations, const char *actorID);

#endif