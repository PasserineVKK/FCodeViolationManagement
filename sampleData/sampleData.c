#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/utils.h"
#include "../include/member.h"
#include "../include/violation.h"
#include "../include/auth.h"
#include "../include/fileio.h"

//đang để status = 1 cho tất cả thành viên để dễ test, có thể chỉnh lại sau
void seedSampleData(Member members[], int *mCount, Violation violations[], int *vCount, Account accounts[], int *aCount) {
    // 3 MEMBERS
    // --- Member 0: Regular member, Academic ---
    strcpy(members[0].fullName,    "Nguyen Van An");
    strcpy(members[0].email,       "anNV@fpt.edu.vn");
    strcpy(members[0].phoneNumber, "0901234561");
    strcpy(members[0].studentID,   "SE200001");
    members[0].team                = 0; // Academic
    members[0].role                = 0; // Member  -> fine = 20,000
    members[0].violationCount      = 3;
    members[0].consecutiveAbsences = 1;
    members[0].totalFine           = 40000.0; // 2 unpaid x 20,000
    members[0].status              = 1;       // Active

    // --- Member 1: Leader, HR ---
    strcpy(members[1].fullName,    "Tran Thi Bich");
    strcpy(members[1].email,       "bichTT@fpt.edu.vn");
    strcpy(members[1].phoneNumber, "0901234562");
    strcpy(members[1].studentID,   "SE200002");
    members[1].team                = 2; // HR
    members[1].role                = 1; // Leader/Vice -> fine = 50,000
    members[1].violationCount      = 3;
    members[1].consecutiveAbsences = 0;
    members[1].totalFine           = 100000.0; // 2 unpaid x 50,000
    members[1].status              = 1;

    // --- Member 2: BCN, Planning ---
    strcpy(members[2].fullName,    "Le Hoang Cuong");
    strcpy(members[2].email,       "cuongLH@fpt.edu.vn");
    strcpy(members[2].phoneNumber, "0901234563");
    strcpy(members[2].studentID,   "SE200003");
    members[2].team                = 1; // Planning
    members[2].role                = 2; // BCN -> fine = 50,000
    members[2].violationCount      = 3;
    members[2].consecutiveAbsences = 2; // Cảnh báo gần Out CLB
    members[2].totalFine           = 50000.0; // 1 unpaid x 50,000
    members[2].status              = 1;

    *mCount = 3;

    // 9 VIOLATIONS (3 per member, mix paid/unpaid)
    // Dùng thời gian cố định để data nhất quán mỗi lần seed
    time_t base = 1745020800; // 2026-04-19 00:00 UTC

    // --- SE200001 (Member, 20,000/lần) ---
    // VIO001: Not uniform — UNPAID
    strcpy(violations[0].violationID, "VIO001");
    strcpy(violations[0].studentID,   "SE200001");
    violations[0].reason        = 0;
    violations[0].violationTime = base - 86400 * 10;
    violations[0].fine          = 20000.0;
    violations[0].isPaid        = 0;
    violations[0].pelnaty       = 0;
    strcpy(violations[0].note, "Forgot uniform on Tuesday");

    // VIO002: Meeting absence — UNPAID
    strcpy(violations[1].violationID, "VIO002");
    strcpy(violations[1].studentID,   "SE200001");
    violations[1].reason        = 1;
    violations[1].violationTime = base - 86400 * 7;
    violations[1].fine          = 20000.0;
    violations[1].isPaid        = 0;
    violations[1].pelnaty       = 0;
    strcpy(violations[1].note, "Absent without notice");

    // VIO003: Not join activity — PAID
    strcpy(violations[2].violationID, "VIO003");
    strcpy(violations[2].studentID,   "SE200001");
    violations[2].reason        = 2;
    violations[2].violationTime = base - 86400 * 3;
    violations[2].fine          = 20000.0;
    violations[2].isPaid        = 1;
    violations[2].pelnaty       = 0;
    strcpy(violations[2].note, "");

    // --- SE200002 (Leader, 50,000/lần) ---
    // VIO004: Not uniform — UNPAID
    strcpy(violations[3].violationID, "VIO004");
    strcpy(violations[3].studentID,   "SE200002");
    violations[3].reason        = 0;
    violations[3].violationTime = base - 86400 * 14;
    violations[3].fine          = 50000.0;
    violations[3].isPaid        = 0;
    violations[3].pelnaty       = 0;
    strcpy(violations[3].note, "");

    // VIO005: Not join activity — UNPAID
    strcpy(violations[4].violationID, "VIO005");
    strcpy(violations[4].studentID,   "SE200002");
    violations[4].reason        = 2;
    violations[4].violationTime = base - 86400 * 8;
    violations[4].fine          = 50000.0;
    violations[4].isPaid        = 0;
    violations[4].pelnaty       = 0;
    strcpy(violations[4].note, "Skipped activity");

    // VIO006: Meeting absence — PAID
    strcpy(violations[5].violationID, "VIO006");
    strcpy(violations[5].studentID,   "SE200002");
    violations[5].reason        = 1;
    violations[5].violationTime = base - 86400 * 2;
    violations[5].fine          = 50000.0;
    violations[5].isPaid        = 1;
    violations[5].pelnaty       = 0;
    strcpy(violations[5].note, "");

    // --- SE200003 (BCN, 50,000/lần) ---
    // VIO007: Not uniform — UNPAID
    strcpy(violations[6].violationID, "VIO007");
    strcpy(violations[6].studentID,   "SE200003");
    violations[6].reason        = 0;
    violations[6].violationTime = base - 86400 * 20;
    violations[6].fine          = 50000.0;
    violations[6].isPaid        = 0;
    violations[6].pelnaty       = 0;
    strcpy(violations[6].note, "");

    // VIO008: Meeting absence — PAID
    strcpy(violations[7].violationID, "VIO008");
    strcpy(violations[7].studentID,   "SE200003");
    violations[7].reason        = 1;
    violations[7].violationTime = base - 86400 * 15;
    violations[7].fine          = 50000.0;
    violations[7].isPaid        = 1;
    violations[7].pelnaty       = 0;
    strcpy(violations[7].note, "");

    // VIO009: Not join activity — PAID
    strcpy(violations[8].violationID, "VIO009");
    strcpy(violations[8].studentID,   "SE200003");
    violations[8].reason        = 2;
    violations[8].violationTime = base - 86400 * 5;
    violations[8].fine          = 50000.0;
    violations[8].isPaid        = 1;
    violations[8].pelnaty       = 0;
    strcpy(violations[8].note, "");

    *vCount = 9;

    // 3 ACCOUNTS
    // Default password = studentID
    strcpy(accounts[0].studentID, "SE200001");
    strcpy(accounts[0].password,  "SE200001");
    accounts[0].role      = 0; // Member
    accounts[0].isLocked  = 0;
    accounts[0].failCount = 0;

    strcpy(accounts[1].studentID, "SE200002");
    strcpy(accounts[1].password,  "SE200002");
    accounts[1].role      = 0; // Member (role chức vụ ở Member struct)
    accounts[1].isLocked  = 0;
    accounts[1].failCount = 0;

    strcpy(accounts[2].studentID, "SE200003");
    strcpy(accounts[2].password,  "SE200003");
    accounts[2].role      = 1; // Admin/BCN
    accounts[2].isLocked  = 0;
    accounts[2].failCount = 0;

    *aCount = 3;

    // Lưu thẳng vào file sau khi seed
    saveToFile("sampleData/members.dat", members, sizeof(Member), *mCount);
    saveToFile("sampleData/violations.dat", violations, sizeof(Violation), *vCount);
    saveToFile("sampleData/accounts.dat", accounts, sizeof(Account), *aCount);
    printf("Created %d members, %d violations, %d accounts in sampleData.\n",
            *mCount, *vCount, *aCount);
}