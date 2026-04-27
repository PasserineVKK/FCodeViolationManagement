#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/utils.h"
#include "../include/member.h"
#include "../include/violation.h"
#include "../include/auth.h"
#include "../include/fileio.h"

void seedSampleData(Member members[], int *mCount,
                    Violation violations[], int *vCount,
                    Account accounts[], int *aCount) {

    time_t base = 1745020800; // 2026-04-19 00:00 UTC

    // =============================================================
    // MEMBERS
    // =============================================================

    strcpy(members[0].fullName,    "Nguyen Van An");
    strcpy(members[0].email,       "anNV@fpt.edu.vn");
    strcpy(members[0].phoneNumber, "0901234561");
    strcpy(members[0].studentID,   "SE200001");
    members[0].team                = 0; 
    members[0].role                = 0; 
    members[0].violationCount      = 3;
    members[0].consecutiveAbsences = 2; 
    members[0].totalFine           = 40000.0;
    members[0].isPending           = 0;

    strcpy(members[1].fullName,    "Tran Thi Bich");
    strcpy(members[1].email,       "bichTT@fpt.edu.vn");
    strcpy(members[1].phoneNumber, "0901234562");
    strcpy(members[1].studentID,   "SE200002");
    members[1].team                = 2; 
    members[1].role                = 1; 
    members[1].violationCount      = 3;
    members[1].consecutiveAbsences = 3; 
    members[1].totalFine           = 100000.0;
    members[1].isPending           = 0; 

    strcpy(members[2].fullName,    "Le Hoang Cuong");
    strcpy(members[2].email,       "cuongLH@fpt.edu.vn");
    strcpy(members[2].phoneNumber, "0901234563");
    strcpy(members[2].studentID,   "SE200003");
    members[2].team                = 1; 
    members[2].role                = 2; 
    members[2].violationCount      = 3;
    members[2].consecutiveAbsences = 0;
    members[2].totalFine           = 50000.0;
    members[2].isPending           = 0;

    *mCount = 3;

    // =============================================================
    // VIOLATIONS
    // =============================================================

    strcpy(violations[0].violationID, "VIO001");
    strcpy(violations[0].studentID,   "SE200001");
    violations[0].reason        = 0;
    violations[0].fine          = 20000.0;
    violations[0].isPaid        = 0;
    violations[0].isPending     = 0;
    violations[0].pelnaty       = 0;
    violations[0].violationTime = base;
    strcpy(violations[0].note, "Forgot uniform");

    strcpy(violations[1].violationID, "VIO002");
    strcpy(violations[1].studentID,   "SE200001");
    violations[1].reason        = 1;
    violations[1].fine          = 20000.0;
    violations[1].isPaid        = 0;
    violations[1].isPending     = 0;
    violations[1].pelnaty       = 0;
    violations[1].violationTime = base + 86400; 
    strcpy(violations[1].note, "Absent without notice");

    strcpy(violations[2].violationID, "VIO003");
    strcpy(violations[2].studentID,   "SE200001");
    violations[2].reason        = 2;
    violations[2].fine          = 20000.0;
    violations[2].isPaid        = 1;
    violations[2].isPending     = 0;
    violations[2].pelnaty       = 0;
    violations[2].violationTime = base + 172800;
    strcpy(violations[2].note, "");

    strcpy(violations[3].violationID, "VIO004");
    strcpy(violations[3].studentID,   "SE200002");
    violations[3].reason        = 1;
    violations[3].fine          = 50000.0;
    violations[3].isPaid        = 0;
    violations[3].isPending     = 0;
    violations[3].pelnaty       = 0;
    violations[3].violationTime = base;
    strcpy(violations[3].note, "Absence 1");

    strcpy(violations[4].violationID, "VIO005");
    strcpy(violations[4].studentID,   "SE200002");
    violations[4].reason        = 1;
    violations[4].fine          = 50000.0;
    violations[4].isPaid        = 0;
    violations[4].isPending     = 0;
    violations[4].pelnaty       = 0;
    violations[4].violationTime = base + 86400;
    strcpy(violations[4].note, "Absence 2");

    strcpy(violations[5].violationID, "VIO006");
    strcpy(violations[5].studentID,   "SE200002");
    violations[5].reason        = 1;
    violations[5].fine          = 50000.0;
    violations[5].isPaid        = 0;
    violations[5].isPending     = 1;
    violations[5].pelnaty       = 1;
    violations[5].violationTime = base + 172800;
    strcpy(violations[5].note, "Absence 3 - pending kick");

    strcpy(violations[6].violationID, "VIO007");
    strcpy(violations[6].studentID,   "SE200003");
    violations[6].reason        = 0;
    violations[6].fine          = 50000.0;
    violations[6].isPaid        = 0;
    violations[6].isPending     = 0;
    violations[6].pelnaty       = 0;
    violations[6].violationTime = base;
    strcpy(violations[6].note, "");

    strcpy(violations[7].violationID, "VIO008");
    strcpy(violations[7].studentID,   "SE200003");
    violations[7].reason        = 1;
    violations[7].fine          = 50000.0;
    violations[7].isPaid        = 1;
    violations[7].isPending     = 0;
    violations[7].pelnaty       = 0;
    violations[7].violationTime = base + 86400;
    strcpy(violations[7].note, "");

    strcpy(violations[8].violationID, "VIO009");
    strcpy(violations[8].studentID,   "SE200003");
    violations[8].reason        = 2;
    violations[8].fine          = 50000.0;
    violations[8].isPaid        = 1;
    violations[8].isPending     = 0;
    violations[8].pelnaty       = 0;
    violations[8].violationTime = base + 172800;
    strcpy(violations[8].note, "");

    *vCount = 9;

    // =============================================================
    // ACCOUNTS
    // =============================================================

    strcpy(accounts[0].studentID, "SE200001");
    strcpy(accounts[0].password,  "SE200001");
    accounts[0].role      = 0; 
    accounts[0].isLocked  = 0;
    accounts[0].failCount = 0;

    strcpy(accounts[1].studentID, "SE200002");
    strcpy(accounts[1].password,  "SE200002");
    accounts[1].role      = 0; 
    accounts[1].isLocked  = 0;
    accounts[1].failCount = 0;

    strcpy(accounts[2].studentID, "SE200003");
    strcpy(accounts[2].password,  "SE200003");
    accounts[2].role      = 1; 
    accounts[2].isLocked  = 0;
    accounts[2].failCount = 0;

    *aCount = 3;

    saveToFile("data/members.dat",    members,    sizeof(Member),    *mCount);
    saveToFile("data/violations.dat", violations, sizeof(Violation), *vCount);
    saveToFile("data/accounts.dat",   accounts,   sizeof(Account),   *aCount);

    printf("Sample data seeded: %d members, %d violations, %d accounts.\n",
           *mCount, *vCount, *aCount);
}