#ifndef VIOLATION_VIEW_H
#define VIOLATION_VIEW_H

#include "../violation.h"

void displayViolationList(Violation violations[], int vCount);

// Features 1.2
void displayViolationByStudentId(char *id, Violation violations[], int vCount);

#endif