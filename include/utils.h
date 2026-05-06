
#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 123456.78 to 123,456.78 VND
int formatCurrency(double amount, char* outputString, size_t availableLen);
// use: declare char outputString[], then call this function.
// after this func operate, the value of this outputString is the desired
// currency format

int normalizeName(char* name);

const char* translateRole(int role);
const char* translateTeam(int team);
const char* translateIsPaid(int isPaid);
const char* translatePenalty(int penalty);
