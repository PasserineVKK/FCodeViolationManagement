#include <stdio.h>
#include <string.h>
#include "../include/member.h"
#include "../include/violation.h"
#include "../include/fileio.h"

//file
int loadViolations(Violation violations[], int *count) {
    return loadFromFile(VIOLATIONS_FILE, violations, sizeof(Violation), MAX_VIOLATIONS, count);
}

int saveViolations(Violation violations[], int count) {
    return saveToFile(VIOLATIONS_FILE, violations, sizeof(Violation), count);
}

