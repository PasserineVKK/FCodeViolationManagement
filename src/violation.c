#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/violation.h"
#include "../include/fileio.h"

#include "../include/view/consoleInput.h"

#include "../include/member.h"

// Reinit
void ensureCapacity(Violation **violations, int *capacity, int count)
{
	if (count >= *capacity)
	{
		int newCapacity = (*capacity == 0) ? 10 : (int)(*capacity * 1.5);
		Violation *tmp = (Violation *)realloc(*violations, newCapacity * sizeof(Violation));
		if (tmp == NULL)
		{
			printf("Allocate error!!!!\n");
			return;
		}
		*violations = tmp;
		*capacity = newCapacity;
	}
}

// file
int loadViolations(Violation **violations, int *count, int *capacity)
{
	int recordCount = countRecordsInFile(VIOLATIONS_FILE, sizeof(Violation));
	if (recordCount < 0)
	{
		*violations = NULL;
		*count = 0;
		*capacity = 0;
		return -1;
	}

	*count = recordCount;
	*capacity = recordCount > 0 ? recordCount : 10;
	*violations = (Violation *)malloc((*capacity) * sizeof(Violation));

	if (*violations == NULL)
	{
		*count = 0;
		*capacity = 0;
		return 0;
	}

	if (recordCount > 0)
	{
		int loaded = 0;
		loadFromFile(VIOLATIONS_FILE, *violations, sizeof(Violation), recordCount, &loaded);
		*count = loaded;
	}
	else
	{
		*count = 0;
	}

	return 1;
}

int saveViolations(Violation *violations, int count)
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

int checkOutCondition(const Violation *violations, int count, const char *id)
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

		if (violations[i].reason == REASON_MEETING_ABSENCE)
		{
			meetingAbsenceCount++;
		}
		else if (violations[i].reason == REASON_VIOLENCE)
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

void refreshFineAfterRolechange(char *memberId, int role, Violation *violations, int count)
{
	for (int i = 0; i < count; i++)
	{
		Violation *v = &violations[i];
		if (strcmp(v->studentID, memberId) == 0 && v->fine != 0)
			v->fine = calculateFine(role, v->reason);
	}
}

// 2.5 Features

Violation *findViolationById(const char *violationId, Violation *violations, int count)
{
	for (int i = 0; i < count; i++)
		if (strcmp(violations[i].violationID, violationId) == 0)
			return &violations[i];
	return NULL;
}

void updateIsPaidField(char *violationId, Violation *violations, int count, int value)
{
	Violation *v = findViolationById(violationId, violations, count);
	if (v == NULL)
	{
		printf("Can not find violation with violation id %s\n", violationId);
		return;
	}

	if (value != NOT_PAY && value != ALREADY_PAID)
	{
		printf("Value must be 0(Not pay) or 1(Already paid)\n");
		return;
	}

	v->isPaid = value;
	updateMemberTotalFine(v->studentID);
}

void createViolationID(int index, char *buffer)
{
	sprintf(buffer, "VI%04d", index);
}

void getReason(int *reason)
{
	printf("Ch\341\273\215n l\303\275 do vi ph\341\272\241m:\n");
	printf("%d - Not uniform\n", REASON_NOT_UNIFORM);
	printf("%d - Meeting absence\n", REASON_MEETING_ABSENCE);
	printf("%d - Not join in Club activity\n", REASON_NO_CLUB_ACTIVITY);
	printf("%d - Violence\n", REASON_VIOLENCE);
	getPosIntInRange("Nh\341\272\255p s\341\273\221: ", reason, REASON_NOT_UNIFORM, REASON_VIOLENCE);
}

void getNote(char *note, int size)
{
	printf("Ghi ch\303\272 (n\341\272\277u c\303\263): ");
	getchar();
	fgets(note, size, stdin);
	note[strcspn(note, "\n")] = '\0';
}

void createNewViolation(Violation **violations, int *count, int *capacity, Member *m)
{
	if (m == NULL)
		return;
	ensureCapacity(violations, capacity, *count);

	Violation *v = &((*violations)[*count]);
	getReason(&v->reason);
	v->violationTime = time(NULL);
	v->fine = calculateFine(m->role, v->reason);

	v->isPaid = NOT_PAY;
	v->isPending = PENDING;
	v->pelnaty = PENALTY_FINANCIAL;

	getNote(v->note, 100);
	createViolationID(*count + 1, v->violationID);

	int confirm;
	inputYesNo(&confirm, "Are your sure to create this violation!");
	if (confirm)
	{
		(*count)++;
		saveViolations(*violations, *count);
	}

	handleSeriousViolation(m, v);
	updateMemberTotalFine(m->studentID);
}

void handleSeriousViolation(Member *m, Violation *v)
{
	int confirm;
	if (v->reason == REASON_MEETING_ABSENCE)
		m->consecutiveAbsences++;

	if (m->consecutiveAbsences == 3)
	{
		char message[100];
		inputYesNo(&confirm, "This student has missed 3 meetings; confirmation of their departure from the club. Yes/No?");
		if (confirm)
		{
			v->pelnaty = PENALTY_KICK;
		}
		else
		{
			// Nothing to do?
		}
	}

	if (v->reason == REASON_VIOLENCE)
		v->pelnaty = PENALTY_KICK;
}

void freeViolations(Violation **violations, int *count, int *capacity)
{
	if (*violations != NULL)
	{
		free(*violations);
		*violations = NULL;
	}
	*count = 0;
	*capacity = 0;
}