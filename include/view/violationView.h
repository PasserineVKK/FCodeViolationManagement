#ifndef VIOLATION_VIEW_H
#define VIOLATION_VIEW_H

#include "../model.h"
#include "../violation.h"
#include "../member.h"

// Display all violations in list
void displayViolationList(const Violation violations[], int vCount);

// Features 1.2: Display violations for a specific student
void displayViolationByStudentId(const char* id, const ViolationList* violations);

// Features 2.5: Mark fine as paid
void markFineAsPaidView(ViolationList* violations, MemberList* members);

// Features 1.3: For member to view their own unpaid fines
void viewMyUnpaidFines(const char* myStudentID, const ViolationList* violations);

// Display violations filtered by time range
void displayViolationsByTimeRange(const ViolationList* violations);

#endif
