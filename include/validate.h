#ifndef VALIDATE_H
#define VALIDATE_H

// ===== I. VALIDATION RELATED TO DATA TYPE ==================================
// 1.1. Check number belong to Z sets
int isPosInteger (const char *s); 

// 1.2. Check number is a real number
int isPosFloat (const char *s); 

// 1.3. Check number belong to N* sets
int isWholeNumber (const char *s);

// 1.4. Check only number 1 or 0 => Use for input yes / no (confirmation)
int isOneOrZero (const char *s);
// =========================================================================


// ===== II. VALIDATION RELATED TO DATE ========================================
// 2.1. Check leap year 
int isLeapYear(int year);

// 2.2. Check time is valid in the PAST
int isValidPastDate(int d, int m, int y);

// 2.3. Check time is valid in the FUTURE
int isValidFutureDate(int d, int m, int y);
// =========================================================================


// ===== III. VALIDATION RELATED TO MEMBER INFORMATION ============================
// 3.0. Beautify name
void beautifyName(char name[]);

// 3.1. Check valid name
int isValidName (const char *s);

// 3.2. Check valid email
int isValidEmail (const char *s);

// 3.3. Check phone
int isValidPhone (const char *s);

// 3.4. Check valid team (only 0 or 1 or 2 or 3)
int isValidTeam (const char *s);

// 3.5. Check valid role (only 0 or 1 or 2)
int isValidRole (const char *s);
// =========================================================================


// ===== IV. VALIDATION RELATED TO ACCOUNT INFORMATION ============================
// 4.1. Check valid member ID
int isValidMemberID (const char *s);

// 4.2. Check valid password
int isValidPasword (const char *s);
// =========================================================================

#endif
