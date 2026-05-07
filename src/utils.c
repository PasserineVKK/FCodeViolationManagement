#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/model.h"
#define VIOLATION_FACTORS 3
#define MEMBER_FACTORS 3
#define TYPE_VIOLATION 1
#define TYPE_MEMBER 2

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

char* getLastWord(char* inputString){
	char* output;
	int len = strlen(inputString);
	for (int i = len - 1; i > 0 ;i--){
		if (isalpha(inputString[i]) && isspace(inputString[i-1])){
			output = &inputString[i];
			return output;
		}
	}
	return inputString;
}



// comparator, accept vio data type
// compare by team, reason, isPaid
// command: TRP
static int violationComparator(Violation* v1, Violation* v2, char* compareCommand){
	int len = (strlen(compareCommand) > VIOLATION_FACTORS) ? VIOLATION_FACTORS : strlen(compareCommand);
	int result = 0;
	for (int i = 0; i < len; i++){
			// upper = ASC, lower = DESC
		char currentCommand = compareCommand[i];
		int direction = isupper(currentCommand) ? 1 : -1 ;
		currentCommand = toupper(currentCommand);
		switch (currentCommand){
			case 'T':	
				result = v1->owner->team - v2->owner->team;
				break;
			case 'R':
				result = v1->reason - v2->reason;
				break;
			case 'P':
				result = v1->isPaid - v2->isPaid;
				break;
			default:
				result = 0;
		}
		// if the first factor is equal, continue to the next, until end of commands
		if (result != 0) return direction*result;
	}
	return 0;
}

// comparator, accept member data type
// compare by team, role, violationCount, totalFine
static int memberComparator(Member* m1, Member* m2, char* compareCommand){
	int len = (strlen(compareCommand) > VIOLATION_FACTORS) ? VIOLATION_FACTORS : strlen(compareCommand);
	int result = 0;
	for (int i = 0; i < len; i++){
			// upper = ASC, lower = DESC
		char currentCommand = compareCommand[i];
		int direction = isupper(currentCommand) ? 1 : -1 ;
		currentCommand = toupper(currentCommand);
		switch (currentCommand){
			case 'T':	
				result = m1->team - m2->team;
				break;
			case 'R':
				result = m1->role - m2->role;
				break;
			case 'V':
				result = m1->violationCount - m2->violationCount;
				break;
			case 'F':
				if (m1->totalFine > m2->totalFine) result = 1;
				else if (m1->totalFine == m2->totalFine) result = 0;
				else result = -1;
				break;
			case 'N':
				result = strcmp(getLastWord(m1->fullName), getLastWord(m2->fullName));
				break;
			default:
				result = 0;
		}
		// if the first factor is equal, continue to the next, until end of commands
		if (result != 0) return direction*result;
	}
	return 0;
}

void sortViolation(ViolationList* list, Violation* sortPointerList[], char* compareCommand){
	if (list == NULL || list->count <= 1) return;
	int count = list->count;
	for (int i = 0; i < count; i++){
		sortPointerList[i] = &list->data[i];
	}
	int minIndex = 0, maxIndex = count - 1;
	int start = 0, end = count - 1;
	for (int i = 0; i < count - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < count; j++) {
            if (violationComparator(sortPointerList[j], sortPointerList[minIndex], compareCommand) < 0) {
                minIndex = j;
            }
        }

        // Swap
        if (minIndex != i) {
            Violation* temp = sortPointerList[minIndex];
            sortPointerList[minIndex] = sortPointerList[i];
            sortPointerList[i] = temp;
        }
    }		
}

void sortMember(MemberList* list, Member* sortPointerList[], char* compareCommand){
	if (list == NULL || list->count <= 1) return;
	int count = list->count;
	for (int i = 0; i < count; i++){
		sortPointerList[i] = &list->data[i];
	}
	
	int minIndex = 0, maxIndex = count - 1;
	int start = 0, end = count - 1;
	for (int i = 0; i < count - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < count; j++) {
            if (memberComparator(sortPointerList[j], sortPointerList[minIndex], compareCommand) < 0) {
                minIndex = j;
            }
        }

        // Swap
        if (minIndex != i) {
            Member* temp = sortPointerList[minIndex];
            sortPointerList[minIndex] = sortPointerList[i];
            sortPointerList[i] = temp;
        }
    }	
	
}



