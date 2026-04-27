#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "../include/validate.h"

// Check integer in range
int isIntegerBelongRange (const char *s, int min, int max) {
	char *endptr;
    long num = strtol(s, &endptr, 10);

    if (endptr == s || *endptr != '\0') {
		return 0;
	}
    
	if (!(num >= min && num <= max)) {
		return 0;
	}

	return 1;
}

// Check double in range
int isDoubleBelongRange (const char *s, double min, double max) {
	char *endptr;
	double num = strtod(s, &endptr);

	// Check if the entire string was a valid number
	if (endptr == s || *endptr != '\0') {
		return 0;
	}

	if (!(num >= min && num <= max)) return 0;

	return 1;
}

// Check leap year
int isLeapYear(int year) {
	return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

//Check valid time
int isValidDate(int d, int m, int y) {

	//Check if year is < 1900
	if (y < 1900) {
		printf("? Invalid date! Please check day/month/year values.");
		return 0;
	}

	//Check valid month
	if (m < 1 || m > 12) {
		printf("? Invalid date! Please check day/month/year values.");
		return 0;
	}

	//Check  valid day
	int maxDay;
	if (m == 2) maxDay = isLeapYear(y) ? 29 : 28;
	else if (m == 4 || m == 6 || m == 9 || m == 11) maxDay = 30;
	else maxDay = 31;

	if (d > maxDay) {
		printf("? Invalid date! Please check day/month/year values.");
		return 0;
	}

	return 1;
}

// Check time is valid in the PAST
int isValidPastDate(int d, int m, int y) {

	if (!isValidDate(d, m, y)) {
		return 0;
	}

	// Get current time
	time_t t = time(NULL);
	struct tm *now = localtime(&t);

	int curD = now->tm_mday;
	int curM = now->tm_mon + 1;
	int curY = now->tm_year + 1900;

	//Check previous time
	if (y > curY ||
	        (y == curY && m > curM) ||
	        (y == curY && m == curM && d > curD)) {
		printf(" Invalid date! Date must be in the past.");
		return 0;
	}

	return 1;
}

// Check time is valid in the FUTURE
int isValidFutureDate(int d, int m, int y) {

	if (!isValidDate(d, m, y)) {
		return 0;
	}

	// Get current time
	time_t t = time(NULL);
	struct tm *now = localtime(&t);

	int curD = now->tm_mday;
	int curM = now->tm_mon + 1;
	int curY = now->tm_year + 1900;

	//Check future time
	if (y < curY ||
	        (y == curY && m < curM) ||
	        (y == curY && m == curM && d < curD)) {
		printf(" Invalid date! Date must be in the future.");
		return 0;
	}

	return 1;
}

// Beautify name
void beautifyName(char name[]) {
	int i = 0, j = 0;
	while (name[i] == ' ') i++;
	int spaceFlag = 0;

	while (name[i] != '\0') {
		if (name[i] != ' ') {
			name[j++] = name[i];
			spaceFlag = 0;
		} else {
			if (spaceFlag == 0) {
				name[j++] = ' ';
				spaceFlag = 1;
			}
		}
		i++;
	}

	if (j > 0 && name[j - 1] == ' ') j--;
	name[j] = '\0';

	int start = 1;
	for (i = 0; name[i] != '\0'; i++) {
		if (start && name[i] >= 'a' && name[i] <= 'z')
			name[i] -= 32;
		else if (!start && name[i] >= 'A' && name[i] <= 'Z')
			name[i] += 32;

		start = (name[i] == ' ');
	}
}

// Check valid name
int isValidName (const char *s) {
	
	//Declaring and initialization temp string
	char name[strlen(s)];
	strcpy (name, s);
	
	//Trim / Upper and lower case
	beautifyName(name);

	int len = strlen(name);

	// Make sure no '\n' buffer in string
	if (len > 0 && name[len - 1] == '\n') {
		name[len - 1] = '\0';
		len--;
	}

	//Check empty name
	if (name[0] == '\0' || len == 0) {
		printf("? Empty name.\n");
		return 0;
	}
	
	//Check name length
	if (len > 50) {
		printf("? Name longer than specified (max 50 characters).\n");
		return 0;
	}

	//Make sure name only digit from a to z or A to Z
	for (int i = 0; name[i] != '\0'; i++) {
		char c = name[i];

		if (!((c >= 'A' && c <= 'Z') ||
		        (c >= 'a' && c <= 'z') ||
		        c == ' ')) {
			printf("? Name contains invalid characters.\n");
			return 0;
		}
	}

	return 1;
}

// Check valid email
int isValidEmail (const char *s){
	
	int count = 0, j = 0;
	
	//Skip blank
	while (s[j] == ' ') j++;
	
	
	//Check valid email character
	for (int i = 0; s[i] != '\0'; i++){
		char c = s[i];
		if (!((c >= 'A' && c <= 'Z') ||
		        (c >= 'a' && c <= 'z') ||
		        c >= '0' && c <= '9' ||
				c == '@' || c == '.')) {
			printf("Email contain invalid character\n");
			return 0;
		}
		if (c == '@') count++;
	}
	
	//Check if don't have '@'
	if (count == 0) return 0;
	
	return 1;
}

//Check phone (only 10 digits)
int isValidPhone (const char *s){
	int i = 0;
	
	//Skip blank
	while (s[i]== ' ') i++;
	
	//Check first digit is '0'
	if (s[i] != '0') {
		printf("Invalid phone number\n");
		return 0;
	}

	if (strlen(s) - i != 10 ){
		printf("Invalid phone number length\n");
	 	return 0;
	
	}

	//Check valid phone number
	for (i; s[i] != '\0'; i++){
		char c = s[i];
		if (!isdigit(c)) {
			printf("Invalid phone number\n");
			return 0;
		}
	}
	
	return 1;
}


// Check valid student ID
int isValidStudentID (const char *s){
	int i = 0, count = 0;
	
	//Skip blank
	while (s[i] == ' ') i++;
	
	if (!(s[i] == 'S')){
		printf("Student ID must start with 'SE' followed by 6 digits\n");
		return 0;
	}
	i++;
	if (!(s[i++] == 'E')) {
		printf("Student ID must be in the format 'SE' followed by 6 digits\n");
		return 0;
	}
	count += 2;
	
	//Check valid member
	for (; s[i] != '\0'; i++){
		char c = s[i];
		if (!isdigit(c)) {
			printf("After 'SE' in student ID, only 6 digits are allowed\n");
			return 0;
		}
		count ++;
	}
	
	if (count != 8){
		printf("Number of digits after 'SE' must be 6\n");
		return 0;
	}
	
	return 1;
}
