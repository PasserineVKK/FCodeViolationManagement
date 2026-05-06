#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 123456789.12 to 123,456,789.12
int formatCurrency(double amount, char* outputString, size_t availableLen){
	char temp[64];
	// 1. Prepare
	
	//parse amount (2 decimal) to string and set to temp
	snprintf(temp, sizeof(temp), "%.2f VND", amount);
	int len = strlen(temp);

	int dotIndex = len - 4 - 3;
	// the number of comma, from begin to the dot
	int commaCount = (dotIndex > 0) ? (dotIndex - 1) / 3 : 0;
	int newLen = len + commaCount; 
	

	// check available len for string
	if ((size_t)newLen > availableLen){
		snprintf(outputString, availableLen, "Error: Format currency");
		return 0;
	}
	
	//j is the current writting index in outputString
	int j = newLen;
	outputString[j--] = '\0';
	int i;

	// 2. Copy 2 digits after the dot and the dot
	for (i = 0; i < 7; i++){
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

	return 1;
	


}

const char *translateRole(int role) {
	switch (role) {
		case 0:
			return "Member";
		case 1:
			return "Leader/Vice";
		case 2:
			return "Board of Directors";
		default:
			return "Unknown Role";
	}
}

const char *translateTeam(int team) {
	switch (team) {
		case 0:
			return "Academic";
		case 1:
			return "Planning";
		case 2:
			return "HR";
		case 3:
			return "Media";
		default:
			return "Unknown Team";
	}
}

const char *translateIsPaid(int isPaid) {
	switch (isPaid) {
		case 0:
			return "Not Paid";
		case 1:
			return "Paid";
		case 2:
			return "not have to pay";	
		default:
			return "Unknown Payment Status";
	}
}

const char *translatePenalty(int penalty) {
	switch (penalty) {
		case 0:
			return "Disciplinary Fine";
		case 1:
			return "Removal From Club";
		default:
			return "Unknown Penalty";
	}

}

int normalizeName(char* name){
    int n = strlen(name);
    int j = 0;
    int writeSpace = 0; // check 
	// i: iterator
	// j: writter. j write letter and space, and decide a letter is upper or lower.
    for (int i = 0; i < n; i++) {
        if (!isspace(name[i])) {
            // if this letter is first letter -> upper
            // if this letter after space -> add space and upper
            if (j == 0 || writeSpace) {
                if (writeSpace) name[j++] = ' ';
                name[j++] = toupper(name[i]);
            } else {
            // if not both first letter and after space -> lower
                name[j++] = tolower(name[i]);
            }
            writeSpace = 0;
        } else {
            // mark that space must be written after
            if (j > 0) writeSpace = 1;
        }
    }
    name[j] = '\0'; // end of full name
    return 1;
}
