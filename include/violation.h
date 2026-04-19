#ifndef VIOLATION_H
#define VIOLATION_H

#include <time.h>

typedef struct
{
	char violationID[50];
	char studentID[8];
	int reason; // 0 = Not uniform, 1 = Meeting absence, 2 = Not join in Club activity, 3 = Violence
	time_t violationTime;
	double fine;
	int isPaid;	 // 0 = Not yet, 1 = Already
	int pelnaty; // 0 = Financial pelnaty, 1 = Kick
	char note[100];
} Violation;

#endif