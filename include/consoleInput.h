#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

//Let the user input in valid datatype form
//The logic: all console input is in string datatype 
// => then validate 
// => then if true, assign value to target variable
// => then if false, throw prompt to let user re-enter


// ===== I. INPUT INT, FLOAT, YES/NO OPTION ================================

// 1.1. Input integer
void inputPosInteger (int *target, const char * prompt);

// 1.2. Input real number
void inputPosFloat (float *target, const char * prompt);

// 1.3 Input yes no option
void inputYesNo (int *option, const char * prompt);
// =========================================================================

// ===== II. INPUT MEMBER INFO ================================
// 2.1 Input member name
void inputMemberName (char *target, const char * prompt);

//2.2 Input email
void inputMemberEmail (char *target, const char * prompt);

// 2.3 Input student ID
void inputStudentID (char *target, const char * prompt);

//2.4 Input new phone number
void inputMemberPhone (char *target, const char * prompt);

// 2.5 Input member role
void inputMemberRole (int *target, const char * prompt);

//2.6 Input member team
void inputMemberTeam (int *target, const char * prompt);

// =========================================================================

//Input password
void inputPassword (char *target, const char * prompt);

#endif