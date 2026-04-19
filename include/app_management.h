#ifndef APP_MANAGEMENT_H
#define APP_MANAGEMENT_H

#endif

#include "violation.h"
#include "member.h"
#include "auth.h"

#define MAX_SUPPORTED 1000

typedef struct
{
    Violation *data;
    int size;
    int capacity;
} ViolationList;

typedef struct
{
    Account accounts[MAX_SUPPORTED];
    int size;
} AccountList;

typedef struct
{
    Member members[MAX_SUPPORTED];
    int size;
} MemberList;
