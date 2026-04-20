#include <stdio.h>
#include <stdlib.h>
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

double calculateFine(int role, int reason){
	double fineAmount = 0;
	if (reason == 3) fineAmount = 0;
	else if (role == 0) fineAmount = 20000;
	else fineAmount = 50000;
	
	return fineAmount;
}

