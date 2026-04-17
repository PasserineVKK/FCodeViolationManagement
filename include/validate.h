#ifndef VALIDATE_H
#define VALIDATE_H

//These functions use for checking vaid only


// ===== I. VALIDATION RELATED TO DATA TYPE ==================================
// 1.1. Check number belong to Z sets
int isInterger (const char *str); 

// 1.2. Check number is a real number
int isFloat (const char *str); 

// 1.3. Check number belong to N* sets
int isWholeNumber (const char *str);

// 1.4. Check only number 1 or 0 => Use for input yes / no (confirmation)
int isOneOrZero (const char *str);
// =========================================================================


// ===== II. VALIDATION RELATED TO DATE ========================================
// 2.1. Check leap year 
int isLeapYear(int year);

// 2.2. Check time is valid in the PAST
int isValidPastDate(int day, int month, int year);

// 2.3. Check time is valid in the FUTURE
int isValidFutureDate(int day, int month, int year);
// =========================================================================


// ===== III. VALIDATION RELATED TO MEMBER INFORMATION ============================
// 3.1. Check valid name
int isValidName (const char *str);

// 3.2. Check valid email
int isValidEmail (const char *str);

// 3.3. Check phone
int isValidPhone (const char *str);

// 3.4. Check valid team (only 0 or 1 or 2 or 3)
int isValidTeam (const char *str);

// 3.5. Check valid role (only 0 or 1 or 2)
int isValidRole (const char *str);
// =========================================================================


// ===== IV. VALIDATION RELATED TO ACCOUNT INFORMATION ============================
// 4.1. Check valid member ID
int isValidMemberID (const char *str);

// 4.2. Check valid password
int isValidPasword (const char *str);
// =========================================================================

#endif
