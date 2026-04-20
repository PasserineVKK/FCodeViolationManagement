#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/member.h"
#include "../include/violation.h"
#include "../include/fileio.h"

const Violation violations[MAX_VIOLATIONS];
int total_violation;
// file
int loadViolations(Violation violations[], int *count)
{
    return loadFromFile(VIOLATIONS_FILE, violations, sizeof(Violation), MAX_VIOLATIONS, count);
}

int saveViolations(Violation violations[], int count)
{
    return saveToFile(VIOLATIONS_FILE, violations, sizeof(Violation), count);
}

double calculateFine(int role, int reason)
{
    double fineAmount = 0;
    if (reason == REASON_VIOLENCE)
        fineAmount = 0;
    else if (role == 0)
        fineAmount = 20000;
    else
        fineAmount = 50000;

    return fineAmount;
}

void refreshFineAfterRolechange(char *id, int role)
{
    for (int i = 0; i < total_violation; i++)
    {
        Violation *v = &violations[i];
        if (strcmp(v->studentID, id) == 0 && v->fine != 0)
            v->fine = calculateFine(role, v->reason);
    }
}