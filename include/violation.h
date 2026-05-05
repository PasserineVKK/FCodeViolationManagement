#ifndef VIOLATION_H
#define VIOLATION_H

#include <stdlib.h>
#include <time.h>

#include "model.h"

#define REASON_NOT_UNIFORM 0
#define REASON_MEETING_ABSENCE 1
#define REASON_NO_CLUB_ACTIVITY 2
#define REASON_VIOLENCE 3

#define PENDING 1
#define NOT_PENDING 0

#define ALREADY_PAID 1
#define NOT_PAY 0

#define PENALTY_FINANCIAL 0
#define PENALTY_KICK 1

#define VIOLATIONS_FILE "data/violations.dat"
#define MAX_VIOLATIONS 3000

void initViolationList(ViolationList *list, int initialCapacity);

int loadViolations(ViolationList *violations, const MemberList *members);
int saveViolations(ViolationList *violations);

double calculateFine(int role, int reason);
void refreshFineAfterRolechange(const char* memberId, int role, ViolationList *violations);

Violation* findViolationById(const char* violationId, ViolationList *violations);

int getViolationIndexById(const ViolationList *violations, const char* violationId);

int searchMemberByIdInV(const ViolationList *violations, const char* id);
void updateIsPaidField(const char* violationId, ViolationList *violations, int value);
int addViolation(ViolationList *violations, const Violation *newV);

void deleteViolation(ViolationList *list, const Violation *v);

void recordViolationView(ViolationList *violations, MemberList *members);

void handleSeriousViolation(const Member *member, const Violation *violation);

void displayWarningList(const MemberList *members, const ViolationList *violations);
void displayKickList(const MemberList *members, const ViolationList *violations);
int isMemberInWarningList(const Member *member, const ViolationList *violations);

int hasViolenceViolation(const char* studentID, const ViolationList *violations);

void removeMemberById(const char* id, MemberList *members, AccountList *accounts, ViolationList *violations);
void checkAndWarnOutClub(MemberList *members, AccountList *accounts, ViolationList *violations);

#endif
