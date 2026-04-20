#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>


// 123456.789 to 123,456.789
int formatCurrency(double amount, char* outputString, size_t availableLen);
// use: declare char outputString[], then call this function.
// after this func operate, the value of this outputString is the desired currency format

int normalizeName(char* name);


