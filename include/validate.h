#ifndef VALIDATE_H
#define VALIDATE_H

//Check integer in range
int isIntegerBelongRange (const char *s, int min, int max) ;

//Check double in range
int isDoubleBelongRange (const char *s, double min, double max);

// Check time is valid in the PAST
int isValidPastDate(int d, int m, int y) ;

// Check time is valid in the FUTURE
int isValidFutureDate(int d, int m, int y);

// Beautify name
void beautifyName(char name[]) ;
// Check valid name
int isValidName (const char *s);

// Check valid email
int isValidEmail (const char *s);

//Check phone (only 10 digits)
int isValidPhone (const char *s);

// Check valid student ID
int isValidStudentID (const char *s);

#endif
