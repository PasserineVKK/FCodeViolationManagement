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

// Displays all violations in a list.
void displayViolationList(const Violation violations[], int vCount);

// Feature 1.2: Displays violations for a specific student.
void displayViolationByStudentId(const char *id, const ViolationList *violations);

// Feature 2.5: Marks a fine as paid.
void markFineAsPaidView(ViolationList *violations, MemberList *members);

// Feature 1.3: Lets members view their own unpaid fines.
void viewMyUnpaidFines(const char *myStudentID, const ViolationList *violations);

// Displays violations filtered by time range.
void displayViolationsByTimeRange(const ViolationList *violations);

void displayViolationManagerMenu();

void flexibleDisplayViolationList(ViolationList* violations, MemberList* members);
void changeFilterOption();

#endif
