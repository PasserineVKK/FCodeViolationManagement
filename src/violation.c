#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/member.h"
#include "../include/violation.h"
#include "../include/fileio.h"

Violation violations[MAX_VIOLATIONS];
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

int checkOutCondition(const Violation violations[], int count, const char *id)
{
	int i;
	int meetingAbsenceCount = 0;
	int violenceCount = 0;

	if (violations == NULL || count <= 0 || id == NULL || id[0] == '\0')
	{
		return 0;
	}

	for (i = 0; i < count; i++)
	{
		if (strcmp(violations[i].studentID, id) != 0)
		{
			continue;
		}

		if (violations[i].reason == 1)
		{
			meetingAbsenceCount++;
		}
		else if (violations[i].reason == 3)
		{
			violenceCount++;
		}

		if (meetingAbsenceCount >= 3 || violenceCount >= 1)
		{
			return 1;
		}
	}

	return 0;
}

void refreshFineAfterRolechange(char *memberId, int role)
{
	for (int i = 0; i < total_violation; i++)
	{
		Violation *v = &violations[i];
		if (strcmp(v->studentID, memberId) == 0 && v->fine != 0)
			v->fine = calculateFine(role, v->reason);
	}
}

// 2.5 Features

Violation *findViolationById(const char *violationId, Violation violations[], int count)
{
	for (int i = 0; i < count; i++)
		if (strcmp(violations[i].violationID, violationId) == 0)
			return &violations[i];
	return NULL;
}

void updateIsPaidField(char *violationId, Violation violations[], int count, int value)
{
	Violation *v = findViolationById(violationId, violations, count);
	if (v == NULL)
	{
		printf("Can not find violation with violation id %s\n", violationId);
		return;
	}

	if (value != NOT_PAY || value != ALREADY_PAID)
	{
		printf("Value must be 0(Not pay) or 1(Already paid)\n");
		return;
	}

	v->isPaid = value;
	updateMemberTotalFine(v->studentID);
}