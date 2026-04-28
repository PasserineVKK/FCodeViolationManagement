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

int loadViolations(Violation violations[], int* count);
int saveViolations(Violation violations[], int count);

int checkOutCondition(const Violation violations[], int count, const char* id);
double calculateFine(int role, int reason);
void refreshFineAfterRolechange(char* memberId, int role,
                                Violation violations[], int count);

Violation* findViolationById(const char* violationId, Violation violations[],
                             int count);

int getViolationIndexById(Violation violations[], int count,
                          const char* violationId);

int searchMemberByIdInV(Violation violations[], int count, const char* id);
void updateIsPaidField(char* violationId, Violation violations[], int count,
                       int value);
int addViolation(Violation violations[], int* count, Violation newV);

void deleteViolation(Violation* violation, int* count, Violation* v);
void updateViolation(Violation* violation);

// Features 2.4: Record Violation
void recordViolationView(Violation violations[], int* vCount, int* vCapacity,
                         Member members[], int mCount);

#endif