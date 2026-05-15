#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

#include <time.h>
// Ensures user input is converted to valid data types.
// Logic: read input as strings, validate, then assign to target variables.
// If validation fails, show the prompt again.

// ---- Primitive input helpers ----
int inputString(char* buf, int size, const char* prompt);
void inputTimeRange(time_t* start, time_t* end, const char* prompt);
void inputValidTime (time_t* target, const char* prompt);

int inputPasswordOrCancel(char* target, const char* prompt);
void inputPassword(char* target, const char* prompt);

// ---- Numeric inputs ----
// Input integer in range
void inputIntegerInRange(int* target, int min, int max, const char* prompt);
// Input double number in range
void inputDoubleInRange(double* target, double min, double max, const char* prompt);
// Input a yes/no option
void inputYesNo(int* option, const char* prompt);

// ---- Member-specific inputs ----
// Input member name
void inputMemberName(char* target, const char* prompt);
// Input email
void inputMemberEmail(char* target, const char* prompt);
// Input student ID
void inputStudentID(char* target, const char* prompt);
// Input new phone number
void inputMemberPhone(char* target, const char* prompt);
// Input member role
void inputMemberRole(int* target, const char* prompt);
// Input member team
void inputMemberTeam(int* target, const char* prompt);

#endif
