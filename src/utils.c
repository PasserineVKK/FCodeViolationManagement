#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 123456789.1234 to 123,456,789.123
void formatCurrency(double amount, char* outputString, size_t availableLen){
	char temp[64];
	// 1. Prepare
	
	//parse amount (3 decimal) to string and set to temp
	snprintf(temp, sizeof(temp), "%.3f VND", amount);
	int len = strlen(temp);

	int dotIndex = len - 4 - 4;
	// the number of comma, from begin to the dot
	int commaCount = (dotIndex > 0) ? (dotIndex - 1) / 3 : 0;
	int newLen = len + commaCount; 
	

	// check available len for string
	if ((size_t)newLen > availableLen){
		snprintf(outputString, availableLen, "Error: Format currency");
		return;
	}
	
	//j is the current writting index in outputString
	int j = newLen;
	outputString[j--] = '\0';
	int i;

	// 2. Copy 3 digits after the dot and the dot
	for (i = 0; i < 8; i++){
		outputString[j--] = temp[len - 1 - i];
	}
	
	// 3. Copy the integer part and insert comma after each triple
	int count = 0;
	for (i = dotIndex-1; i >= 0; i--){
		if (count > 0 && count%3 == 0){
			outputString[j--] = ',';
		}
		outputString[j--] = temp[i];
		count++;
	}
	
	
}
