#ifndef VIOLATION_VIEW_H
#define VIOLATION_VIEW_H

#include "../violation.h"

void displayViolationList(Violation violation[], int size);
void displayViolationByStudentId(char *id, Violation violations[], int size);

void displayViolationsByPelnatyStatus(Violation violations[], int size, int pelnatyStatus);

void displayViolationsByReason(Violation violations[], int size, int reason);

void displayViolationsByPaidStatus(Violation violations[], int size, int isPaid);

#endif