#ifndef VIOLATION_VIEW_H
#define VIOLATION_VIEW_H

#include "../violation.h"

void displayViolationList(Violation violation[], int size);
void displayViolationByStudentId(char *id, Violation violations[], int size);

#endif