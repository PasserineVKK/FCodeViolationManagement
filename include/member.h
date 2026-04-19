#ifndef MEMBER_H
#define MEMBER_H

#include "violation.h"
typedef struct
{
	char fullName[100];
	char email[100];
	char phoneNumber[11];
	char studentID[8]; // SE000000
	int team;		   // 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
	int role;		   // 0 = Member, 1 = Leader/Vice, 2 = BOD
	int consecutiveAbsences;
	double totalFine;
	int status;
} Member;

#endif