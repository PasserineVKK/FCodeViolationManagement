#ifndef VIOLATION_VIEW_H
#define VIOLATION_VIEW_H

#include "../model.h"
#include "../violation.h"
#include "../member.h"

#define V_FILTER_ID 0 // Base filter
#define V_FILTER_TEAM 1
#define V_FILTER_REASON 2
#define V_FILTER_PAID 3
// #define V_FILTER_NAME -1
#define V_FILTER_TIME_RANGE 4

#define ASC 1
#define DESC 0

// Display all violations in list
void displayViolationList(const Violation violations[], int vCount);

// Features 1.2: Display violations for a specific student
void displayViolationByStudentId(const char *id, const ViolationList *violations);

// Features 2.5: Mark fine as paid
void markFineAsPaidView(ViolationList *violations, MemberList *members);

// Features 1.3: For member to view their own unpaid fines
void viewMyUnpaidFines(const char *myStudentID, const ViolationList *violations);

// Display violations filtered by time range
void displayViolationsByTimeRange(const ViolationList *violations);

void displayViolationManagerMenu();

void flexibleDisplayViolationList(ViolationList* violations, MemberList* members);
void changeFilterOption();

#endif
