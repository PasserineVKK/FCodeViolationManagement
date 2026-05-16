#ifndef VALIDATE_H
#define VALIDATE_H

// Checks if an integer string is in range.
int isIntegerBelongRange(const char *s, int min, int max);

// Checks if a double string is in range.
int isDoubleBelongRange(const char *s, double min, double max);

int isValidDate(int d, int m, int y);

// Checks if a date is valid and in the past.
int isValidPastDate(int d, int m, int y);

// Checks if a date is valid and in the future.
int isValidFutureDate(int d, int m, int y);

// Checks if a name is valid.
int isValidName(const char *s);

// Checks if an email is valid.
int isValidEmail(const char *s);

// Checks if a phone number is valid (10 digits only).
int isValidPhone(const char *s);

// Checks if a student ID is valid.
int isValidStudentID(const char *s);

int isBlank(const char *str);

#endif