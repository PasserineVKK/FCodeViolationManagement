#ifndef VIOLATION_VIEW_H
#define VIOLATION_VIEW_H

#include "../member.h"
#include "../violation.h"

void displayViolationList(Violation violations[], int vCount);

// Features 1.2
void displayViolationByStudentId(char* id, Violation violations[], int vCount);

// Features 2.4: Record Violation
void recordViolationView(Violation violations[], int* vCount, Member members[],
                         int mCount);

// Features 2.5: Mark fine as paid

void markFineAsPaidView(Violation violations[], int vCount, Member members[],
                        int mCount);

void viewMyUnpaidFines(const char* myStudentID, Violation violations[],
                       int vCount);

void displayViolationsByTimeRange(Violation violations[], int vCount);

#endif