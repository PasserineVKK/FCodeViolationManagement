
#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/model.h"

// 123456.78 to 123,456.78 VND

int formatCurrency(double amount, char* outputString, size_t availableLen);

// Usage: declare char outputString[], then call this function.
// After this function executes, outputString contains the formatted
// currency value.

int normalizeName(char *name);

const char* translateRole(int role);
const char* translateTeam(int team);
const char* translateIsPaid(int isPaid);
const char* translatePenalty(int penalty);

char* getLastWord(char* inputString);

// Declare sortPointerList first, then pass it as an output parameter.
// compare command:
	// reason: r DESC, R ASC
	// paid: p DESC, P ASC
	// team: t DESC, T ASC
	// valid command examples: rtp, prt, PrT, tR, R, P, T, t
void sortViolation(ViolationList* list, Violation* sortPointerList[], char* compareCommand);

void sortMember(MemberList* list, Member* sortPointerList[], char* compareCommand);



