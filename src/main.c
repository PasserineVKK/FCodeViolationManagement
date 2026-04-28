#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "../include/auth.h"
#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/report.h"
#include "../include/utils.h"
#include "../include/validate.h"
#include "../include/view/memberView.h"
#include "../include/view/viewUtil.h"
#include "../include/view/violationView.h"
#include "../include/violation.h"

// #include "../sampleData/sampleData.h"

void seedSampleData(Member members[], int* mCount, Violation violations[],
                    int* vCount, Account accounts[], int* aCount) {
    // =============================================================
    // 3 MEMBERS
    // =============================================================

    // --- Member 0: Regular member, Academic, no violations ---
    strcpy(members[0].fullName, "Nguyen Van An");
    strcpy(members[0].email, "anNV@fpt.edu.vn");
    strcpy(members[0].phoneNumber, "0901234561");
    strcpy(members[0].studentID, "SE200001");
    members[0].team = 0;  // Academic
    members[0].role = 0;  // Member -> fine = 20,000
    members[0].violationCount = 3;
    members[0].consecutiveAbsences = 1;
    members[0].totalFine = 40000.0;
    members[0].isPending = 0;

    // --- Member 1: Leader, HR, absent 3 times in a row -> pending kick ---
    strcpy(members[1].fullName, "Tran Thi Bich");
    strcpy(members[1].email, "bichTT@fpt.edu.vn");
    strcpy(members[1].phoneNumber, "0901234562");
    strcpy(members[1].studentID, "SE200002");
    members[1].team = 2;  // HR
    members[1].role = 1;  // Leader/Vice -> fine = 50,000
    members[1].violationCount = 3;
    members[1].consecutiveAbsences = 2;
    members[1].totalFine = 100000.0;
    members[1].isPending = 1;

    // --- Member 2: BCN, Planning, warning ---
    strcpy(members[2].fullName, "Le Hoang Cuong");
    strcpy(members[2].email, "cuongLH@fpt.edu.vn");
    strcpy(members[2].phoneNumber, "0901234563");
    strcpy(members[2].studentID, "SE200003");
    members[2].team = 1;  // Planning
    members[2].role = 2;  // BCN -> fine = 50,000
    members[2].violationCount = 3;
    members[2].consecutiveAbsences = 2;
    members[2].totalFine = 50000.0;
    members[2].isPending = 0;

    *mCount = 3;

    // =============================================================
    // 9 VIOLATIONS (3 per member, mix paid/unpaid)
    // =============================================================
    time_t base = 1745020800;  // 2026-04-19 00:00 UTC

    // SE200001
    for (int i = 0; i < 3; i++) {
        strcpy(violations[i].studentID, "SE200001");
        violations[i].fine = 20000.0;
        violations[i].isPending = 0;
        violations[i].penalty = 0;
    }
    strcpy(violations[0].violationID, "VIO001");
    violations[0].reason = 0;
    violations[0].isPaid = 0;
    strcpy(violations[0].note, "Forgot uniform");

    strcpy(violations[1].violationID, "VIO002");
    violations[1].reason = 1;
    violations[1].isPaid = 0;
    strcpy(violations[1].note, "Absent without notice");

    strcpy(violations[2].violationID, "VIO003");
    violations[2].reason = 2;
    violations[2].isPaid = 1;

    // SE200002
    for (int i = 3; i < 6; i++) {
        strcpy(violations[i].studentID, "SE200002");
        violations[i].fine = 50000.0;
        violations[i].isPaid = 0;
        violations[i].penalty = 1;
    }
    strcpy(violations[3].violationID, "VIO004");
    violations[3].isPending = 0;
    strcpy(violations[3].note, "Absence 1");

    strcpy(violations[4].violationID, "VIO005");
    violations[4].isPending = 0;
    strcpy(violations[4].note, "Absence 2");

    strcpy(violations[5].violationID, "VIO006");
    violations[5].isPending = 1;
    strcpy(violations[5].note, "Absence 3 - pending kick");

    // SE200003
    for (int i = 6; i < 9; i++) {
        strcpy(violations[i].studentID, "SE200003");
        violations[i].fine = 50000.0;
        violations[i].isPending = 0;
        violations[i].penalty = 0;
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
    strcpy(accounts[0].password, "SE200001");
    accounts[0].role = 0;  // Member
    accounts[0].isLocked = 0;
    accounts[0].failCount = 0;

    strcpy(accounts[1].studentID, "SE200002");
    strcpy(accounts[1].password, "SE200002");
    accounts[1].role = 0;
    accounts[1].isLocked = 0;
    accounts[1].failCount = 0;

    strcpy(accounts[2].studentID, "SE200003");
    strcpy(accounts[2].password, "SE200003");
    accounts[2].role = 1;  // Admin/BCN
    accounts[2].isLocked = 0;
    accounts[2].failCount = 0;

    *aCount = 3;

    // save to files
    saveToFile("data/members.dat", members, sizeof(Member), *mCount);
    saveToFile("data/violations.dat", violations, sizeof(Violation), *vCount);
    saveToFile("data/accounts.dat", accounts, sizeof(Account), *aCount);

    printf("Created sample data with Pending status in data/ folder.\n");
}

/* run this program using the console pauser or add your own getch,
 * system("pause") or input loop */

void config() { SetConsoleOutputCP(65001); }

int main(int argc, char* argv[]) {
    config();

    int mCount = 1000, vCount = 1000,
        aCount = 1000;  // set to max, then reset later
    int vCapacity = 3000;
    char studentID[10];

    // set the array to be empty.
    Member members[MAX_MEMBERS] = {0};
    Violation* violations = malloc(sizeof(Violation) * vCapacity);
    Account accounts[MAX_ACCOUNTS] = {0};

    seedSampleData(members, &mCount, violations, &vCount, accounts, &aCount);

    // mCount = loadMembers(members, &mCount);
    // vCount = loadViolations(violations, &vCount);
    // aCount = loadAccounts(accounts, &aCount);

    // loginRole represent the role of this account
    // menuRole represent which menu will be open.

    int loginRole = -1, menuRole = -1;
    int choice = -1, isStayLogin = 0;
    // isStayedLogin check whether user is auth-ed. 0 = No, 1 = Yes;
    int mIndex = -1, vIndex = -1;

    do {
        // Firstly, auth. If stay login ==> do not check
        //				  If not stay login =>> check again.
        if (isStayLogin == 0) {
            loginRole = login(accounts, studentID, aCount);
            if (loginRole == -1) {
                int isExit = 0;
                inputYesNo(&isExit,
                           "Student ID does not exist, do you want to exit? \n "
                           "1. Yes\n 0. No: ");
                if (isExit == 1)
                    return 0;
                else
                    displayNotificationByMemberID(members[mIndex].studentID,
                                                  ADMIN_WARNING);
                pauseProgram();
                continue;
            } else {
                menuRole = loginRole;
                mIndex = searchMemberByIdInM(members, mCount, studentID);
                vIndex = searchMemberByIdInV(violations, vCount, studentID);
                isStayLogin = 1;
                // login successfully ==> Assign value for menuRole to open
                // menu, and memberIndex to identify user
            }
        }

        // clearScreen();
        switch (menuRole) {
            case 0: {
                printf(
                    "\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"
                    "\n┃                 MEMBER MENU                  ┃"
                    "\n┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫"
                    "\n┃  1. View Profile                             ┃"
                    "\n┃  2. View Violation History                   ┃"
                    "\n┃  3. View Total Unpaid Fines                  ┃"
                    "\n┃  4. View Club Member List                    ┃"
                    "\n┃  5. Reset Password                           ┃"
                    "\n┃  6. Log Out                                  ┃"
                    "\n┃  7. Exit                                     ┃"
                    "\n┃  8. Switch to Admin Menu                     ┃"
                    "\n┃  9.  View notification                       ┃"
                    "\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");

                inputIntegerInRange(&choice, 1, 8,
                                    "==> Enter your selection: ");

                clearScreen();
                switch (choice) {
                    case 1:
                        displayOneMemberInfo(members[mIndex]);
                        break;
                    case 2:
                        displayViolationByStudentId(studentID, violations,
                                                    vCount);
                        break;
                    case 3:
                        // not finish yet
                        break;
                    case 4:
                        displayMemberList(members, mCount);
                        break;
                    case 5:
                        printf("===== CHANGE PASSWORD =====\n");
                        changePassword(accounts, aCount, studentID, menuRole);
                        break;
                    case 6:
                        isStayLogin = 0;
                        loginRole = -1;
                        menuRole = -1;
                        // mark as not login, reset menu role
                        continue;
                    case 7:
                        // if choice = 7, save the data end return.
                        return 0;
                    case 8:
                        if (loginRole == 1 || loginRole == 2) {
                            menuRole = 1;
                            continue;
                        } else {
                            printf("Permission denied. Try again.\n");
                            break;
                        }
                    case 9:
                        displayNotificationByMemberID(members[mIndex].studentID,
                                                      IGNORE_NOTI_TYPE);
                        break;
                    default:
                        printf("Invalid option, please try again.\n");
                }
                break;
            }

            case 1:
            case 2: {
                printf(
                    "\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"
                    "\n┃                  ADMIN MENU                  ┃"
                    "\n┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫"
                    "\n┃  1.  Add New Member                          ┃"
                    "\n┃  2.  Edit Member Information                 ┃"
                    "\n┃  3.  Remove Member                           ┃"
                    "\n┃  4.  Create new violation                    ┃"
                    "\n┃  5.  Mark Fine as Paid                       ┃"
                    "\n┃  6.  View Violation List                     ┃"
                    "\n┃  7.  Statistics by Department                ┃"
                    "\n┃  8.  View Member in Sorted List              ┃"
                    "\n┃  9.  Change Member's Password                ┃"
                    "\n┃ 10.  Log Out                                 ┃"
                    "\n┃ 11.  Exit                                    ┃"
                    "\n┃ 12.  Switch to Member Menu                   ┃"
                    "\n┃ 13.  Create global notification              ┃",
                    "\n┃ 14.  Delete  violation                       ┃"
                    "\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
                inputIntegerInRange(&choice, 1, 14,
                                    " ==> Enter your selection: ");

                clearScreen();
                switch (choice) {
                    case 1:
                        addMember(members, &mCount, accounts, &aCount);
                        break;
                    case 2:
                        updateMember(members, &mCount, violations, vCount);
                        break;
                    case 3:
                        removeMember(members, &mCount, accounts, &aCount,
                                     violations, &vCount);
                        break;
                    case 4:
                        recordViolationView(violations, &vCount, &vCapacity,
                                            members, mCount);
                        break;
                    case 5:
                        markFineAsPaidView(violations, vCount, members, mCount);
                        break;
                    case 6:
                        displayViolationList(violations, vCount);
                        // not sorted by team, role yet
                        break;
                    case 7:
                        showTotalFineByRole(members, mCount);
                        break;
                    case 8: {
                        int sortMode;
                        inputYesNo(
                            &sortMode,
                            "Sort mode: \n 1. ASC\n 0. DESC\n Your choice: ");
                        if (sortMode == 0) sortMode = -1;
                        // sortMode only accept 1 as ASC or -1 asc DESC.
                        displayInSortByVioCount(members, mCount, sortMode);
                        break;
                    }
                    case 9: {
                        changePassword(accounts, aCount, studentID, menuRole);
                        break;
                    }

                    case 10:
                        isStayLogin = 0;
                        loginRole = -1;
                        menuRole = -1;
                        // mark as not login, reset menu role
                        continue;
                    case 11:
                        return 0;
                    case 12:
                        menuRole = 0;
                        continue;
                    case 13:
                        char content[200];
                        inputString(content, 200,
                                    "Input global notification content: ");
                        globalNotification(content);
                        break;
                    case 14:
                        char violationId[10];
                        inputString(violationId, 10, "Enter violation id");
                        Violation* v =
                            findViolationById(violationId, violations, vCount);
                        deleteViolation(violations, &vCount, v);
                        break;
                    default:
                        printf("Invalid option, please try again.");
                        break;
                }
                break;
            }
        }
        pauseProgram();
    } while (1);
    return 0;
}
