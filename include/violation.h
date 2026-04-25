#ifndef VIOLATION_H
#define VIOLATION_H

#include <time.h>
#include <stdlib.h>

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

#include "model.h"

int loadViolations(Violation **violations, int *count, int *capacity);

int saveViolations(Violation *violations, int count);

int checkOutCondition(const Violation *violations, int count, const char *id);

double calculateFine(int role, int reason);

void refreshFineAfterRolechange(char *memberId, int role, Violation *violations, int count);

Violation *findViolationById(const char *violationId, Violation *violations, int count);

void updateIsPaidField(char *violationId, Violation *violations, int count, int value);

void createNewViolation(Violation **violations, int *count, int *capacity, Member *m);

void freeViolations(Violation **violations, int *count, int *capacity);

#endif
