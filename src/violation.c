#include <stdio.h>
#include <stdlib.h>

double calculateFine(int role, int reason){
	double fineAmount = 0;
	if (reason == 3) fineAmount = 0;
	else if (role == 0) fineAmount = 20000;
	else fineAmount = 50000;
	
	return fineAmount;
}
