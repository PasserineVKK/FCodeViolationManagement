#ifndef MODEL_H
#define MODEL_H

typedef struct
{
    char fullName[100];
    char email[100];
    char phoneNumber[11];
    char studentID[10]; // SE000000\0
    int team;           // 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
    int role;           // 0 = Member, 1 = Leader/Vice, 2 = BOD
    int violationCount;
    int consecutiveAbsences;
    double totalFine;
    int isPending; // 0 = not pending, 1 = pending
} Member;

typedef struct
{
    char studentID[10]; // SE000000\0
    char password[30];
    int role;      // 0 = Member, 1 = BOD
    int isLocked;  // 1 = This account locked after 3 failed trials
    int failCount; // consecutive failed trials
} Account;

typedef struct
{
    char violationID[50];
    char studentID[10]; // SE000000\0
    int reason;
    time_t violationTime;
    double fine;
    int isPaid;
    int pelnaty;
    char note[100];
    int isPending;
} Violation;

#endif