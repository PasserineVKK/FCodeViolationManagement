#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "../include/validate.h"

// ===== I. VALIDATION RELATED TO DATA TYPE ==================================
// 1.1. Check number belong to Z sets
int isPosInteger (const char *s){
    int i = 0;
    
    //Skip blank
    while (s[i] == ' ') i++;

    //Check negative
    if (s[i] == '-') return 0;

    if (s[i] == '+') i++;

    // Check digit
    int digitFound = 0;

    for (; s[i] != '\0'; i++) {
        if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
        digitFound = 1;
    }

    return digitFound;
}

// 1.2. Check number is a real number
int isPosFloat (const char *s){
    int i = 0, dotCount = 0, digitCount = 0;

    //Skip blank
    while (s[i] == ' ') i++;

    //Check negative
    if (s[i] == '-') return 0;

    //Check digit and dot
    for (; s[i] != '\0'; i++) {
        if (s[i] == '.') {
            dotCount++;
            if (dotCount > 1)
                return 0;
        }
        else if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
        else {
            digitCount++;
        }
    }

    return digitCount > 0;
}

// 1.3. Check number belong to N* sets
int isWholeNumber (const char *str);

// 1.4. Check only number 1 or 0 => Use for input yes / no (confirmation)
int isOneOrZero (const char *str);
// =========================================================================


// ===== II. VALIDATION RELATED TO DATE ========================================
// 2.1. Check leap year 
int isLeapYear(int year);

// 2.2. Check time is valid in the PAST
int isValidPastDate(int d, int m, int y) {

    if (d <= 0 || m <= 0 || y <= 0) {
        printf("? Invalid date! Please check day/month/year values.");
        return 0;
    }

    if (y < 1900) {
        printf("? Invalid date! Please check day/month/year values.");
        return 0;
    }

    if (m < 1 || m > 12) {
        printf("? Invalid date! Please check day/month/year values.");
        return 0;
    }

    int maxDay;
    if (m == 2) maxDay = isLeapYear(y) ? 29 : 28;
    else if (m == 4 || m == 6 || m == 9 || m == 11) maxDay = 30;
    else maxDay = 31;

    if (d > maxDay) {
        printf("? Invalid date! Please check day/month/year values.");
        return 0;
    }

    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    int curD = now->tm_mday;
    int curM = now->tm_mon + 1;
    int curY = now->tm_year + 1900;

    if (y > curY ||
       (y == curY && m > curM) ||
       (y == curY && m == curM && d > curD))
    {
        printf("? Invalid date! Please check day/month/year values.");
        return 0;
    }

    int age = curY - y;
    if (curM < m || (curM == m && curD < d))
        age--;

    if (age < 18) {
        printf("? Invalid date of birth! Student must be at least 18 years old.\n");
        return 0;
    }

    printf("? Valid date\n");
    return 1;
}

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