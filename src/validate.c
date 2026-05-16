#include "../include/validate.h"
#include "../include/model.h"
#include "../include/utils.h"
#include "../include/view/viewUtil.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VIOLATION_FACTORS 3
#define MEMBER_FACTORS 3

// Check integer in range
int isIntegerBelongRange(const char *s, int min, int max)
{
    char *endptr;
    long num = strtol(s, &endptr, 10);

    if (endptr == s || *endptr != '\0')
    {
        return 0;
    }

    if (!(num >= min && num <= max))
    {
        return 0;
    }

    return 1;
}

// Check double in range
int isDoubleBelongRange(const char *s, double min, double max)
{
    char *endptr;
    double num = strtod(s, &endptr);

    // Check if the entire string was a valid number
    if (endptr == s || *endptr != '\0')
    {
        return 0;
    }

    if (!(num >= min && num <= max))
        return 0;

    return 1;
}

// Check leap year
int isLeapYear(int year)
{
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

// Check valid time
int isValidDate(int d, int m, int y)
{
    // Check if year is < 1900
    if (y < 1900)
    {
        uiError("Invalid date. Check day, month, and year.\n"); 
        return 0;
    }

    // Check valid month
    if (m < 1 || m > 12)
    {
        uiError("Invalid date. Check day, month, and year.\n");
        return 0;
    }

    // Check  valid day
    int maxDay;
    if (m == 2)
        maxDay = isLeapYear(y) ? 29 : 28;
    else if (m == 4 || m == 6 || m == 9 || m == 11)
        maxDay = 30;
    else
        maxDay = 31;

    if (d > maxDay)
    {
        uiError("Invalid date. Check day, month, and year.\n");
        return 0;
    }

    return 1;
}

// Check time is valid in the PAST
int isValidPastDate(int d, int m, int y)
{
    if (!isValidDate(d, m, y))
    {
        return 0;
    }

    // Get current time
    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    int curD = now->tm_mday;
    int curM = now->tm_mon + 1;
    int curY = now->tm_year + 1900;

    // Check previous time
    if (y > curY || (y == curY && m > curM) ||
        (y == curY && m == curM && d > curD))
    {
        uiError("Invalid date. Must be in the past.\n");
        return 0;
    }

    return 1;
}

// Check time is valid in the FUTURE
int isValidFutureDate(int d, int m, int y)
{
    if (!isValidDate(d, m, y))
    {
        return 0;
    }

    // Get current time
    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    int curD = now->tm_mday;
    int curM = now->tm_mon + 1;
    int curY = now->tm_year + 1900;

    // Check future time
    if (y < curY || (y == curY && m < curM) ||
        (y == curY && m == curM && d < curD))
    {
        uiError("Invalid date. Must be in the future.\n");
        return 0;
    }

    return 1;
}

// Check valid name
int isValidName(const char *s)
{
    // Declaring and initialization temp string
    char name[strlen(s)];
    strcpy(name, s);

    // Trim / Upper and lower case
    normalizeName(name);

    int len = strlen(name);

    // Make sure no '\n' buffer in string
    if (len > 0 && name[len - 1] == '\n')
    {
        name[len - 1] = '\0';
        len--;
    }

    // Check empty name
    if (name[0] == '\0' || len == 0)
    {
        uiError("Name is empty.\n");
        return 0;
    }

    // Check name length
    if (len > 50)
    {
        uiError("Name is too long (max 50 chars).\n");
        return 0;
    }

    // Make sure name only digit from a to z or A to Z
    for (int i = 0; name[i] != '\0'; i++)
    {
        char c = name[i];

        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' '))
        {
            uiError("Name has invalid characters.\n");
            return 0;
        }
    }

    return 1;
}

// Check valid email
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isValidEmail(const char *email) {
    int i, atCount = 0;
    int len = strlen(email);

    // Minimum length check
    if (len < 5)
        return 0;

    // Can't start or end with '@' or '.'
    if (email[0] == '@' || email[0] == '.' ||
        email[len - 1] == '@' || email[len - 1] == '.')
        return 0;

    for (i = 0; i < len; i++) {

        // Count @
        if (email[i] == '@') {
            atCount++;
        }

        // Blank not allow
        if (isspace(email[i])) {
            return 0;
        }

        // Valid charararacters 
        if (!(isalnum(email[i]) ||
              email[i] == '@' ||
              email[i] == '.' ||
              email[i] == '_' ||
              email[i] == '-' ||
              email[i] == '+')) {
            return 0;
        }

        // Allow ".."
        if (i < len - 1 && email[i] == '.' && email[i + 1] == '.') {
            return 0;
        }
    }

    // Only 1 @
    if (atCount != 1)
        return 0;

    // // Dot '.' must after @
    char *atPtr = strchr(email, '@');
    char *dotPtr = strrchr(email, '.');

    if (atPtr == NULL || dotPtr == NULL)
        return 0;
    if (dotPtr < atPtr)
        return 0;

    // At least 2 character after last dot
    if ((email + len - dotPtr) <= 2){
        uiError("Email must have at least 2 characters after the last dot.\n");
        return 0;
    }    
    return 1;
}

// Check phone (only 10 digits)
int isValidPhone(const char *s)
{
    int i = 0;

    // Skip blank
    while (s[i] == ' ')
        i++;

    // Check first digit is '0'
    if (s[i] != '0')
    {
        uiError("Invalid phone number.\n");
        return 0;
    }

    if (strlen(s) - i != 10)
    {
        uiError("Phone number must be 10 digits.\n");
        return 0;
    }

    // Check valid phone number
    for (i; s[i] != '\0'; i++)
    {
        char c = s[i];
        if (!isdigit(c))
        {
            uiError("Invalid phone number.\n");
            return 0;
        }
    }

    return 1;
}

// Check valid student ID
int isValidStudentID(const char *s)
{
    int i = 0, count = 0;

    // Skip blank
    while (s[i] == ' ')
        i++;

    if (!(s[i] == 'S' ||
          s[i] == 'H' ||
          s[i] == 'Q' ||
          s[i] == 'D' ||
          s[i] == 'C' ))
    {
        uiError("Invalid first character in student ID\n");
        return 0;
    }
    i++;
    if (!(s[i] == 'S' ||
          s[i] == 'E' ||
          s[i] == 'A' ))
    {
        uiError("Invalid second character in student ID\n");
        return 0;
    }
    i++;
    count += 2;

    while (isdigit((unsigned char)s[i])){
        count++;
        i++;
    }
    while (isspace((unsigned char)s[i])) i++;

    if (s[i] != '\0'){
        uiError("Student ID must contain only digits after prefix\n");
        return 0;
    }

    if (count != 8)
    {
        uiError("Student ID must contain 2 prefix characters exactly 6 digits\n");
        return 0;
    }

    return 1;
}

int isBlank(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        if (!isblank(str[i]))
            return 0;
    return 1;
}
