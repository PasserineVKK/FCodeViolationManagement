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

    // =============================================================
    // 3 MEMBERS
    // =============================================================

    // --- Member 0: Regular member, Academic, bình thường ---
    strcpy(members[0].fullName,    "Nguyen Van An");
    strcpy(members[0].email,       "anNV@fpt.edu.vn");
    strcpy(members[0].phoneNumber, "0901234561");
    strcpy(members[0].studentID,   "SE200001");
    members[0].team                 = 0; // Academic
    members[0].role                 = 0; // Member -> fine = 20,000
    members[0].violationCount       = 3;
    members[0].consecutiveAbsences  = 1;
    members[0].totalFine            = 40000.0; 
    members[0].isPending            = 0;       

    // --- Member 1: Leader, HR, vắng 3 buổi liên tiếp -> chờ BCN confirm kick ---
    strcpy(members[1].fullName,    "Tran Thi Bich");
    strcpy(members[1].email,       "bichTT@fpt.edu.vn");
    strcpy(members[1].phoneNumber, "0901234562");
    strcpy(members[1].studentID,   "SE200002");
    members[1].team                 = 2; // HR
    members[1].role                 = 1; // Leader/Vice -> fine = 50,000
    members[1].violationCount       = 3;
    members[1].consecutiveAbsences  = 3; // Đủ điều kiện Out CLB
    members[1].totalFine            = 100000.0; 
    members[1].isPending            = 1;  // Đang chờ xử lý kick

    // --- Member 2: BCN, Planning, cảnh báo gần Out ---
    strcpy(members[2].fullName,    "Le Hoang Cuong");
    strcpy(members[2].email,       "cuongLH@fpt.edu.vn");
    strcpy(members[2].phoneNumber, "0901234563");
    strcpy(members[2].studentID,   "SE200003");
    members[2].team                 = 1; // Planning
    members[2].role                 = 2; // BCN -> fine = 50,000
    members[2].violationCount       = 3;
    members[2].consecutiveAbsences  = 2; 
    members[2].totalFine            = 50000.0; 
    members[2].isPending            = 0;

    *mCount = 3;

    // =============================================================
    // 9 VIOLATIONS (3 per member, mix paid/unpaid)
    // =============================================================
    time_t base = 1745020800; // 2026-04-19 00:00 UTC

    // SE200001
    for(int i=0; i<3; i++) {
        strcpy(violations[i].studentID, "SE200001");
        violations[i].fine = 20000.0;
        violations[i].isPending = 0;
        violations[i].pelnaty = 0;
    }
    strcpy(violations[0].violationID, "VIO001");
    violations[0].reason = 0; violations[0].isPaid = 0; strcpy(violations[0].note, "Forgot uniform");
    
    strcpy(violations[1].violationID, "VIO002");
    violations[1].reason = 1; violations[1].isPaid = 0; strcpy(violations[1].note, "Absent without notice");

    strcpy(violations[2].violationID, "VIO003");
    violations[2].reason = 2; violations[2].isPaid = 1;

    // SE200002 (vắng 3 buổi liên tiếp)
    for(int i=3; i<6; i++) {
        strcpy(violations[i].studentID, "SE200002");
        violations[i].fine = 50000.0;
        violations[i].isPaid = 0; // Chưa đóng tiền nên mới cộng dồn fine
        violations[i].pelnaty = 1; // Đánh dấu lỗi chuyên cần
    }
    strcpy(violations[3].violationID, "VIO004");
    violations[3].isPending = 0; strcpy(violations[3].note, "Absence 1");

    strcpy(violations[4].violationID, "VIO005");
    violations[4].isPending = 0; strcpy(violations[4].note, "Absence 2");

    strcpy(violations[5].violationID, "VIO006");
    violations[5].isPending = 1; // Lần vắng thứ 3 chốt hạ trạng thái pending
    strcpy(violations[5].note, "Absence 3 - pending kick");

    // SE200003
    for(int i=6; i<9; i++) {
        strcpy(violations[i].studentID, "SE200003");
        violations[i].fine = 50000.0;
        violations[i].isPending = 0;
        violations[i].pelnaty = 0;
    }
    strcpy(violations[6].violationID, "VIO007");
    violations[6].isPaid = 0;

    strcpy(violations[7].violationID, "VIO008");
    violations[7].isPaid = 1;

    strcpy(violations[8].violationID, "VIO009");
    violations[8].isPaid = 1;

    *vCount = 9;

    // =============================================================
    // 3 ACCOUNTS (default password = studentID)
    // =============================================================
 
    strcpy(accounts[0].studentID, "SE200001");
    strcpy(accounts[0].password,  "SE200001");
    accounts[0].role      = 0; // Member
    accounts[0].isLocked  = 0;
    accounts[0].failCount = 0;
 
    strcpy(accounts[1].studentID, "SE200002");
    strcpy(accounts[1].password,  "SE200002");
    accounts[1].role      = 0;
    accounts[1].isLocked  = 0;
    accounts[1].failCount = 0;
 
    strcpy(accounts[2].studentID, "SE200003");
    strcpy(accounts[2].password,  "SE200003");
    accounts[2].role      = 1; // Admin/BCN
    accounts[2].isLocked  = 0;
    accounts[2].failCount = 0;
 
    *aCount = 3;

    //Lưu dữ liệu mẫu vào file
    saveToFile("sampleData/members.dat", members, sizeof(Member), *mCount);
    saveToFile("sampleData/violations.dat", violations, sizeof(Violation), *vCount);
    saveToFile("sampleData/accounts.dat", accounts, sizeof(Account), *aCount);

    printf("Created sample data with Pending status in sampleData/ folder.\n");
}