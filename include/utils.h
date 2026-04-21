#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

// 123456.789 to 123,456.789
void formatCurrency(double amount, char* outputString, size_t availableLen);

const char *translateRole(int role);
const char *translateTeam(int team);
const char *translateIsPaid(int isPaid);
const char *translatePenalty(int penalty);
