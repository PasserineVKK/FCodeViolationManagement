#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "../include/auth.h"
#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/model.h"
#include "../include/report.h"
#include "../include/utils.h"
#include "../include/validate.h"
#include "../include/view/memberView.h"
#include "../include/view/viewUtil.h"
#include "../include/view/violationView.h"
#include "../include/violation.h"

#define REASON_NOT_UNIFORM 0
#define REASON_MEETING_ABSENCE 1
#define REASON_NO_CLUB_ACTIVITY 2
#define REASON_VIOLENCE 3

#define PENDING 1
#define NOT_PENDING 0

#define ALREADY_PAID 1
#define NOT_PAY 0

#define PENALTY_FINANCIAL 0
#define PENALTY_KICK 1

// #include "../sampleData/sampleData.h"

void seedSampleData(MemberList *members, ViolationList *violations, AccountList *accounts) {
    // =============================================================
    // 3 MEMBERS
    // =============================================================

    // --- Member 0: Regular member, Academic, no violations ---
    strcpy(members->data[0].fullName, "Nguyen Van An");
    strcpy(members->data[0].email, "anNV@fpt.edu.vn");
    strcpy(members->data[0].phoneNumber, "0901234561");
    strcpy(members->data[0].studentID, "SE200001");
    members->data[0].team = 0;  // Academic
    members->data[0].role = 0;  // Member -> fine = 20,000
    members->data[0].violationCount = 2;
    members->data[0].consecutiveAbsences = 1;
    members->data[0].totalFine = 40000.0;
    members->data[0].isPending = 0;

    // --- Member 1: Leader, HR, absent 3 times in a row -> pending kick ---
    strcpy(members->data[1].fullName, "Tran Thi Bich");
    strcpy(members->data[1].email, "bichTT@fpt.edu.vn");
    strcpy(members->data[1].phoneNumber, "0901234562");
    strcpy(members->data[1].studentID, "SE200002");
    members->data[1].team = 2;  // HR
    members->data[1].role = 1;  // Leader/Vice -> fine = 50,000
    members->data[1].violationCount = 3;
    members->data[1].consecutiveAbsences = 3;
    members->data[1].totalFine = 150000.0;
    members->data[1].isPending = 1;

    // --- Member 2: BCN, Planning, warning ---
    strcpy(members->data[2].fullName, "Le Hoang Cuong");
    strcpy(members->data[2].email, "cuongLH@fpt.edu.vn");
    strcpy(members->data[2].phoneNumber, "0901234563");
    strcpy(members->data[2].studentID, "SE200003");
    members->data[2].team = 1;  // Planning
    members->data[2].role = 2;  // BCN -> fine = 50,000
    members->data[2].violationCount = 4;
    members->data[2].consecutiveAbsences = 1;
    members->data[2].totalFine = 200000.0;
    members->data[2].isPending = 0;

    // C?p nh?t s? lu?ng
    members->count = 3;

    // =============================================================
    // 9 VIOLATIONS (3 per member, mix paid/unpaid)
    // =============================================================
    time_t base = 1745020800;  // 2026-04-19 00:00 UTC

    // SE200001
    for (int i = 0; i < 2; i++) {
        strcpy(violations->data[i].studentID, members->data[0].studentID);
        violations->data[i].fine = 20000.0;
        violations->data[i].penalty = 0;
        violations->data[i].owner = &members->data[0];
        violations->data[i].owner->isPending = 0;
    }
    strcpy(violations->data[0].violationID, "VIO001");
    violations->data[0].reason = REASON_MEETING_ABSENCE;
    violations->data[0].isPaid = 0;
    strcpy(violations->data[0].note, "Retrospective meeting");

    strcpy(violations->data[1].violationID, "VIO002");
    violations->data[1].reason = REASON_NOT_UNIFORM;
    violations->data[1].isPaid = 0;
    strcpy(violations->data[1].note, "05/05/2026, Tuesday");

    // SE200002
    for (int i = 2; i < 5; i++) {
        strcpy(violations->data[i].studentID, members->data[1].studentID);
        violations->data[i].isPaid = ALREADY_PAID;
        violations->data[i].penalty = PENALTY_FINANCIAL;
        violations->data[i].owner = &members->data[1];
    }
    
    strcpy(violations->data[2].violationID, "VIO003");
    violations->data[2].reason = REASON_MEETING_ABSENCE;
    violations->data[2].fine = 50000.0;
    strcpy(violations->data[2].note, "Retrospective meeting");
    
    strcpy(violations->data[3].violationID, "VIO004");
    violations->data[3].reason = REASON_MEETING_ABSENCE;
    violations->data[3].fine = 50000.0;
    strcpy(violations->data[3].note, "SUMMER semester meeting");

    strcpy(violations->data[4].violationID, "VIO005");
    violations->data[4].reason = REASON_MEETING_ABSENCE;
    strcpy(violations->data[4].note, "Daily report meeting");
    violations->data[4].penalty = PENALTY_KICK;
    violations->data[4].fine = 0;

    // SE200003
    for (int i = 5; i < 9; i++) {
        strcpy(violations->data[i].studentID, members->data[2].studentID);
        violations->data[i].fine = 50000.0;
        violations->data[i].penalty = 0;
        violations->data[i].owner = &members->data[2];
        violations->data[i].owner->isPending = 0;
    }
    
    strcpy(violations->data[5].violationID, "VIO006");
    violations->data[5].isPaid = 0;
    violations->data[5].reason = REASON_NOT_UNIFORM;
    
    strcpy(violations->data[6].violationID, "VIO007");
    violations->data[6].isPaid = 0;
    violations->data[6].reason = REASON_NOT_UNIFORM;

    strcpy(violations->data[7].violationID, "VIO008");
    violations->data[7].isPaid = 1;
    violations->data[7].reason = REASON_NOT_UNIFORM;

    strcpy(violations->data[8].violationID, "VIO009");
    violations->data[8].isPaid = 1;
    violations->data[8].reason = REASON_MEETING_ABSENCE;

    violations->count = 9;
    
    for(int i=0;i<=8;i++){
    	violations->data[i].violationTime = base;
	}

    // =============================================================
    // 3 ACCOUNTS (default password = studentID)
    // =============================================================

    strcpy(accounts->data[0].studentID, "SE200001");
    strcpy(accounts->data[0].password, "SE200001");
    accounts->data[0].role = 0;  // Member
    accounts->data[0].isLocked = 0;
    accounts->data[0].failCount = 0;

    strcpy(accounts->data[1].studentID, "SE200002");
    strcpy(accounts->data[1].password, "SE200002");
    accounts->data[1].role = 1; //Vice or Leader
    accounts->data[1].isLocked = 0;
    accounts->data[1].failCount = 0;

    strcpy(accounts->data[2].studentID, "SE200003");
    strcpy(accounts->data[2].password, "SE200003");
    accounts->data[2].role = 2;  // Admin/BOD
    accounts->data[2].isLocked = 0;
    accounts->data[2].failCount = 0;

  
    accounts->count = 3;
     saveMembers(members);
     saveAccounts(accounts);
     saveViolations(violations);

    printf("Created sample data with Pending status in data/ folder.\n");
}
/* run this program using the console pauser or add your own getch,
 * system("pause") or input loop */

void config() { SetConsoleOutputCP(65001); }

int main(int argc, char* argv[]) {
    config();
    char studentID[10];

    // set the array to be empty.
    MemberList members;
    ViolationList violations;
    initViolationList(&violations, 3000);
    AccountList accounts;

    initNotificationList();
    autoDeleteOutDateNotification();

    seedSampleData(&members, &violations, &accounts);
    loadMembers(&members);
	loadViolations(&violations, &members);
	loadAccounts(&accounts);

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
            loginRole = login(&accounts, studentID);
            // If loginRole = -2 => Student ID not found
            if (loginRole == -2) {
                int isExit = 0;
                inputYesNo(
                    &isExit,
                    "\nDo you want to exit? \n1. Yes\n 0. No\n=>Your choice: ");
                printf("\n");
                if (isExit == 1)
                    return 0;
                else
                    continue;
            }
            // If loginRole = -1 => Account locked
            else if (loginRole == -1) {
                int isExit = 0;
                inputYesNo(
                    &isExit,
                    "\nDo you want to exit? \n1. Yes\n 0. No\n=>Your choice: ");
                printf("\n");
                if (isExit == 1)
                    return 0;
                else
                    continue;
            } else {
                menuRole = loginRole;
		        mIndex = searchMemberByIdInM(&members, studentID);
                vIndex = getViolationIndexById(&violations, studentID);

                isStayLogin = 1;
                // login successfully ==> Assign value for menuRole to open
                // menu, and memberIndex to identify user

                displayNotificationByMemberID(members.data[mIndex].studentID,
                                              ADMIN_WARNING);
                pauseProgram();
                clearScreen();
            }
        }

        clearScreen();
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
                    "\n┃  9. View notification                        ┃"
                    "\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");

                inputIntegerInRange(&choice, 1, 9,
                                    "==> Enter your selection: ");

                clearScreen();
                switch (choice) {
                    case 1:
                        displayOneMemberInfo(members.data[mIndex]);
                        break;
                    case 2:
                        displayViolationByStudentId(studentID, &violations);
                        break;
                    case 3:
                        viewMyUnpaidFines(studentID, &violations);
                        break;
                    case 4:
                        displayMemberList(members.data, members.count);
                        break;
                    case 5:
                        changePassword(&accounts, studentID, menuRole);
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
                        if ((loginRole == 1 || loginRole == 2) && members.data[mIndex].isPending == 1){
                            printf("Pending account. Can only use member menu now. ");
                            break;
                        }
                        if ((loginRole == 1 || loginRole == 2) && members.data[mIndex].isPending != 1) {
                            menuRole = 1;
                            continue;
                        } else {
                            printf("Permission denied. Try again.\n");
                            break;
                        }
                    case 9:
                        displayNotificationByMemberID(members.data[mIndex].studentID,
                                                      IGNORE_NOTI_TYPE);
                        displayGlobalNotification();
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
                    "\n┃  4.  Record new violation                    ┃"
                    "\n┃  5.  Mark Fine as Paid                       ┃"
                    "\n┃  6.  View Violation List                     ┃"
                    "\n┃  7.  Statistics by Department                ┃"
                    "\n┃  8.  Check warning/kick list                 ┃"
                    "\n┃  9.  View Member in Sorted List              ┃"
                    "\n┃ 10.  Change Member's Password                ┃"
                    "\n┃ 11.  View Violations by Time Range           ┃"
                    "\n┃ 12.  Log Out                                 ┃"
                    "\n┃ 13.  Exit                                    ┃"
                    "\n┃ 14.  Switch to Member Menu                   ┃"
                    "\n┃ 15.  Add new notification                    ┃"
                    "\n┃ 16.  Delete violation                        ┃"
                    "\n┃ 17.  Update notification                     ┃"
                    "\n┃ 18.  Delete notification                     ┃"
                    "\n┃ 19.  Show all notifications                  ┃"
                    "\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
                inputIntegerInRange(&choice, 1, 19,
                                    " ==> Enter your selection: ");

                clearScreen();
                switch (choice) {
                    case 1:
                        addMember(&members, &accounts);
                        break;
                    case 2:
                        updateMember(&members, &violations, studentID);
                        break;
                    case 3:
                        removeMember(&members, &accounts, &violations, studentID);
                        break;
                    case 4:
                        recordViolationView(&violations, &members);
                        break;
                    case 5:
                        markFineAsPaidView(&violations, &members);
                        break;
                    case 6:
                        displayViolationList(violations.data, violations.count);
                        // not sorted by team, role yet
                        break;
                    case 7:
                        showFineStatsByTeam(&members, &violations);
                        break;
                    case 8:
                        checkAndWarnOutClub(&members, &accounts, &violations, studentID);
                        break;
                    case 9: {
                        int sortMode;
                        inputYesNo(
                            &sortMode,
                            "Sort mode: \n 1. ASC\n 0. DESC\n Your choice: ");
                        if (sortMode == 0) sortMode = -1;
                        // sortMode only accept 1 as ASC or -1 asc DESC.
                        displayInSortByVioCount(members.data, members.count, sortMode);
                        break;
                    }
                    case 10: {
                        changePassword(&accounts, studentID, menuRole);
                        break;
                    }
                    case 11: {
                        displayViolationsByTimeRange(&violations);
                        break;
                    }

                    case 12:
                        isStayLogin = 0;
                        loginRole = -1;
                        menuRole = -1;
                        // mark as not login, reset menu role
                        continue;
                    case 13:
                        return 0;
                    case 14:
                        menuRole = 0;
                        continue;
                    case 15: {
                        int type;
                        inputIntegerInRange(
                            &type, 0, 2,
                            "Enter type of notification, global(0), notice "
                            "(1), warning (2): ");
                        char content[200];
                        inputString(content, 200, "Enter notify content: ");
                        if (type == ADMIN_NOTICE || type == ADMIN_WARNING) {
                            char memberId[6];
                            inputString(memberId, 6, "Enter member id: ");
                            createNotification(memberId, type, content,
                                               time(NULL) + BASE_DELETE_TIME,
                                               1);
                        } else {
                            createNotification(NULL, type, content,
                                               time(NULL) + BASE_DELETE_TIME,
                                               1);
                        }
                        break;
                    }
                    case 16: {
                        
                        deleteViolation(&violations);
                        break;
                    }
                    case 17: {
                        char id[6];
                        inputString(id, 6, "Enter notification id: ");
                        Notification* n = findNotificationById(id);

                        if (n == NULL) break;

                        int updateType;
                        inputIntegerInRange(
                            &updateType, 0, 2,
                            "Enter type of notification, global(0), notice "
                            "(1), warning (2): ");
                        char updateContent[200];
                        inputString(updateContent, 200,
                                    "Enter notify content: ");
                        char memberId[6];
                        inputString(memberId, 6, "Enter member id: ");
                        if (strcmp(memberId, "") == 0) break;
                        updateNotification(n, memberId, updateType,
                                           updateContent, n->deleteTime);
                        break;
                    }
                    case 18: {
                        char removeId[6];
                        inputString(removeId, 6, "Enter remove notification id: ");
                        deleteNotification(findNotificationById(removeId));
                        printf("Delete notification");
                        break;
                    }
                    case 19: 
                        displayNotificationList();
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

    freeNotificationList();
    return 0;
}
