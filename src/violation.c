#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "member.h"
#include "violation.h"
#include "fileio.h"
#include "consoleInput.h"

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
    if (reason == REASON_VIOLENCE)
        return 0;
    if (role == 0)
        return 20000;
    return 50000;
}

int checkOutCondition(const Violation violations[], int count, const char *id)
{
    int meetingAbsenceCount = 0;
    int violenceCount = 0;

    if (violations == NULL || count <= 0 || id == NULL || id[0] == '\0')
        return 0;

    for (int i = 0; i < count; i++)
    {
        if (strcmp(violations[i].studentID, id) != 0)
            continue;

        if (violations[i].reason == REASON_MEETING_ABSENCE)
            meetingAbsenceCount++;
        else if (violations[i].reason == REASON_VIOLENCE)
            violenceCount++;

        if (meetingAbsenceCount >= 3 || violenceCount >= 1)
            return 1;
    }
    return 0;
}

void refreshFineAfterRolechange(char *memberId, int role, Violation violations[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(violations[i].studentID, memberId) == 0 && violations[i].isPaid == NOT_PAY)
            violations[i].fine = calculateFine(role, violations[i].reason);
    }
}

Violation *findViolationById(const char *violationId, Violation violations[], int count)
{
    for (int i = 0; i < count; i++)
        if (strcmp(violations[i].violationID, violationId) == 0)
            return &violations[i];
    return NULL;
}

int getViolationIndexById(Violation violations[], int count, const char *violationId)
{
    for (int i = 0; i < count; i++)
        if (strcmp(violations[i].violationID, violationId) == 0)
            return i;
    return -1;
}

int searchMemberByIdInV(Violation violations[], int count, const char *id)
{
    for (int i = 0; i < count; i++)
        if (strcmp(violations[i].studentID, id) == 0)
            return i;
    return -1;
}

void getReason(int *reason)
{
    printf("Choose violate reason: \n");
    printf("%d - Not uniform\n", REASON_NOT_UNIFORM);
    printf("%d - Meeting absence\n", REASON_MEETING_ABSENCE);
    printf("%d - Not join in Club activity\n", REASON_NO_CLUB_ACTIVITY);
    printf("%d - Violence\n", REASON_VIOLENCE);
    inputIntegerInRange(reason, 0, 3, "Please enter: ");
}

int addViolation(Violation violations[], int *count, Violation newV)
{
    if (*count >= MAX_VIOLATIONS)
        return 0;
    violations[*count] = newV;
    (*count)++;
    return 1;
}

void updateIsPaidField(char *violationId, Violation violations[], int count, int value)
{
    Violation *v = findViolationById(violationId, violations, count);
    if (v != NULL)
        v->isPaid = value;
}

void ensureCapacity(Violation **violations, int *count, int *capacity)
{
}

void handleSeriousViolation(Member *m, Violation *v)
{
}

void createViolationID(int index, char *buffer)
{
    sprintf(buffer, "VI%04d", index);
}

void createNewViolation(Violation **violations, int *count, int *capacity, Member *m)
{
    if (m == NULL)
        return;
    ensureCapacity(violations, capacity, count);

    Violation *v = &((*violations)[*count]);
    getReason(&v->reason);
    v->violationTime = time(NULL);
    v->fine = calculateFine(m->role, v->reason);

    v->isPaid = NOT_PAY;
    v->isPending = PENDING;
    v->penalty = PENALTY_FINANCIAL;

    inputString(v->note, 50, "Any note? Please input: ");
    createViolationID(*count + 1, v->violationID);
    strcpy(v->studentID, m->studentID);

    int confirm;
    inputYesNo(&confirm, "Are your sure to create this violation! Enter Yes (1), No (0): ");
    if (confirm)
    {
        (*count)++;
        saveViolations(*violations, *count);
    }

    handleSeriousViolation(m, v);
}

void deleteViolation(Violation *violations, int *count, Violation *v)
{
    if (violations == NULL || v == NULL || *count <= 0)
        return;

    for (int i = 0; i < *count; i++)
    {
        if (&violations[i] == v)
        {
            for (int j = i; j < *count - 1; j++)
            {
                violations[j] = violations[j + 1];
            }
            (*count)--;
            break;
        }
    }
}