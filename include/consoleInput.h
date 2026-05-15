#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

#include <time.h>
// Let the user input in valid datatype form
// The logic: all console input is in string datatype
//  => then validate
//  => then if true, assign value to target variable
//  => then if false, throw prompt to let user re-enter

// Input integer in range
void inputIntegerInRange(int* target, int min, int max, const char* prompt);

// Input double number in range
void inputDoubleInRange(double* target, double min, double max,
                        const char* prompt);

// Input yes no option
void inputYesNo(int* option, const char* prompt);

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

// Input password
int inputPasswordOrCancel(char* password, int len, const char* prompt);

int inputString(char* buf, int size, const char* prompt);

void inputTimeRange(time_t* start, time_t* end, const char* prompt);

void inputValidTime (time_t* target, const char* prompt);

void getReason(int *reason);

#endif
