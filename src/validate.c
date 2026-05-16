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

// Normalize name formatting
void beautifyName(char name[])
{
    int i = 0, j = 0;
    while (name[i] == ' ')
        i++;
    int spaceFlag = 0;

    while (name[i] != '\0')
    {
        if (name[i] != ' ')
        {
            name[j++] = name[i];
            spaceFlag = 0;
        }
        else
        {
            if (spaceFlag == 0)
            {
                name[j++] = ' ';
                spaceFlag = 1;
            }
        }
        i++;
    }

    if (j > 0 && name[j - 1] == ' ')
        j--;
    name[j] = '\0';

    int start = 1;
    for (i = 0; name[i] != '\0'; i++)
    {
        if (start && name[i] >= 'a' && name[i] <= 'z')
            name[i] -= 32;
        else if (!start && name[i] >= 'A' && name[i] <= 'Z')
            name[i] += 32;

        start = (name[i] == ' ');
    }
}

// Check valid name
int isValidName(const char *s)
{
    // Declaring and initialization temp string
    char name[strlen(s)];
    strcpy(name, s);

    // Trim / Upper and lower case
    beautifyName(name);

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
int isValidEmail(const char *s)
{
    int count = 0, j = 0;

    // Skip blank
    while (s[j] == ' ')
        j++;

    // Check valid email character
    for (int i = 0; s[i] != '\0'; i++)
    {
        char c = s[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
              c >= '0' && c <= '9' || c == '@' || c == '.'))
        {
            uiError("Email has invalid characters.\n");
            return 0;
        }
        if (c == '@')
            count++;
    }

    // Check if '@' is missing
    if (count == 0){
        uiError("Email is missing '@' symbol.\n");
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
