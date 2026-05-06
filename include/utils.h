
#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 123456.789 to 123,456.789 VN�
int formatCurrency(double amount, char *outputString, size_t availableLen);
// use: declare char outputString[], then call this function.
// after this func operate, the value of this outputString is the desired
// currency format

int normalizeName(char *name);

const char *translateRole(int role);
const char *translateTeam(int team);
const char *translateIsPaid(int isPaid);
const char *translatePenalty(int penalty);

char *getLastWord(char *inputString);

// declare a sortPointerList first, then use it as OUTPUT param
// compare command:
// reason: r DESC, R ASC
// paid: p DESC, P ASC
// team: t DESC, T ASC
// valid command example: rtp, prt, PrT, tR, R, P, T, t,..
void sortViolation(ViolationList *list, Violation *sortPointerList[], char *compareCommand);