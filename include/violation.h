#include <time.h>
#ifndef VIOLATION_H
#define VIOLATION_H
#define MAX_VIOLATIONS 5000
#define VIOLATIONS_FILE "data/violations.dat"

#include <time.h>

#define REASON_NOT_UNIFORM 0
#define REASON_MEETING_ABSENCE 1
#define REASON_NO_CLUB_ACTIVITY 2
#define REASON_VIOLENCE 3

#define PENDING 1
#define NOT_PENDING 0

#define ALREADY_PAID 1
#define NOT_PAY 0

#define PENALTY_FINANCIAL 0
#define PENALTY_KICK 1

typedef struct
{
	char violationID[50];
	char studentID[10]; // SE000000\0
	int reason;			// 0 = Not uniform, 1 = Meeting absence, 2 = Not join in Club activity, 3 = Violence
	time_t violationTime;
	double fine;
	int isPaid;	 // 0 = Not yet, 1 = Already
	int pelnaty; // 0 = Financial pelnaty, 1 = Kick
	char note[100];
	int isPending; // 0 = Not pending, 1 = Pending
} Violation;

// file
int loadViolations(Violation violations[], int *count);
int saveViolations(Violation violations[], int count);
int checkOutCondition(const Violation violations[], int count, const char *id);

double calculateFine(int role, int reason);
void refreshFineAfterRolechange(char *memberId, int role);

Violation *findViolationById(const char *violationId, Violation violations[], int count);
void updateIsPaidField(char *violationId, Violation violations[], int count, int value);
void createNewViolation(Violation violations[], int count, Member *m);

#endif
