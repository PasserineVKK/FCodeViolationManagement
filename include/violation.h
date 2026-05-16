#ifndef VIOLATION_H
#define VIOLATION_H

#include <stdlib.h>
#include <time.h>

#include "model.h"

#define MAX_3_DIGIT_HEX 4095

#define REASON_NOT_UNIFORM 0
#define REASON_MEETING_ABSENCE 1
#define REASON_NO_CLUB_ACTIVITY 2
#define REASON_VIOLENCE 3

#define PENDING 1
#define NOT_PENDING 0

#define ALREADY_PAID 1
#define NOT_PAY 0
#define NOT_HAVE_TO_PAY 2

#define PENALTY_FINANCIAL 0
#define PENALTY_KICK 1

#define VIOLATIONS_FILE "data/violations.dat"
#define MAX_VIOLATIONS 3000

// Initialize and file I/O
void initViolationList(ViolationList *list, int initialCapacity);

int loadViolations(ViolationList *violations, MemberList *members);
int saveViolations(ViolationList *violations);

// Finders / Accessors
Violation *findViolationById(const char *violationId, ViolationList *violations);
int getViolationIndexById(const ViolationList *violations, const char *violationId);

// Calculations and refresh
double calculateFine(int role);
void refreshFineAfterRolechange(const char *memberId, int role, ViolationList *violations);

// CRUD operations
int addViolation(ViolationList *violations, const Violation *newV);
void updateIsPaidField(const char *violationId, ViolationList *violations, int value);
void deleteViolation(ViolationList *violations);

// Views and recording
void recordViolationView(ViolationList *violations, MemberList *members, int actorIndex);
void simpleDisplayViolation(const Violation *v);

// Serious-violation handlers and checks
void handleSeriousViolation(const Member *member, const Violation *violation);
int hasViolenceViolation(const char *studentID, const ViolationList *violations);

// Lists and displays
void displayWarningList(const MemberList *members, const ViolationList *violations);
void displayKickList(const MemberList *members, const ViolationList *violations);
int isMemberInWarningList(const Member *member, const ViolationList *violations);

// Member-related cleanup due to violations
void removeMemberById(const char *id, MemberList *members, AccountList *accounts, ViolationList *violations);
void checkAndWarnOutClub(MemberList *members, AccountList *accounts, ViolationList *violations, const char *actorID);

#endif
