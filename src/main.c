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


// Root user
const unsigned char ADMIN_USER[] = {0x53, 0x45, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00};

// Root password
const unsigned char ADMIN_PASS[] = {0x4C, 0x45, 0x54, 0x4D, 0x45, 0x43, 0x00};

// Performs basic console configuration (UTF-8 and ANSI colors).
void config()
{
    SetConsoleOutputCP(65001);
    enableAnsiColors();
}

// Creates a small set of sample members, violations, and accounts.
// Used to populate `data/` during development or initial runs.
void seedSampleData(MemberList *members, ViolationList *violations, AccountList *accounts);

// Program entry point. Orchestrates module initialization, login flow, and the main menu loop.
int main(int argc, char *argv[])
{

    config();
    char studentID[10];

    // set the array to be empty.
    MemberList members;
    ViolationList violations;
    initViolationList(&violations, 3000);
    AccountList accounts;

    initNotificationList();
    autoDeleteOutDateNotification();

    loadMembers(&members);
    loadViolations(&violations, &members);
    loadAccounts(&accounts);

    // seedSampleData(&members, &violations, &accounts);
    // loginRole stores the current user's role.
    // menuRole stores the active menu.
    int loginRole = -1, menuRole = -1;
    int choice = -1, isStayLogin = 0;
    // isStayLogin tells whether the user stays logged in.
    int mIndex = -1, vIndex = -1;

    int isRunning = 1;
    do
    {
        // If there is no data in .dat, trigger root admin account initialization
        if (accounts.count == 0 && violations.count == 0 && members.count == 0 && isStayLogin == 0){
            uiWarning("CAN NOT FIND EXIST DATA!!! ENTRY BY ROOT ADMIN ACCOUNT\n");
            char rootUsername[64]; 
            char rootPassword[64];
            inputStudentID(rootUsername, "ENTER ROOT USERNAME: ");
            inputPassword(rootPassword, "ENTER ROOT PASSWORD: ");
            if (strcmp(rootUsername, (char*)ADMIN_USER) == 0 && strcmp(rootPassword, (char*)ADMIN_PASS) == 0){
                strcpy(members.data[0].studentID, rootUsername);
                members.data[0].role = 2;
                members.count++;
                strcpy(accounts.data[0].password, rootPassword);
                strcpy(accounts.data[0].studentID, rootUsername);
                accounts.count++;
                uiWarning("CREATE FIRST ADMIN. BE CAREFUL, YOU MUST SET FIRST ADMIN AS BOD\n");
                addMember(&members, &accounts, rootUsername);
                removeOneMember(&members, &accounts, &violations, rootUsername, rootUsername);
                saveAccounts(&accounts);
                saveMembers(&members);
                uiSuccess("CREATE FIRST ADMIN SUCCESSFULLY\n");
            } else return 0;
        }

        // Check login again only if the user is not staying logged in.
        if (isStayLogin == 0)
        {
            loginRole = login(&accounts, studentID);
            // -2 means the student ID was not found.
            if (loginRole == -2)
            {
                int isExit = 0;
                inputYesNo(
                    &isExit,
                    "\nExit now?\n1. Yes\n0. No\n=> Your choice: ");
                printf("\n");
                if (isExit == 1)
                    return 0;
                else
                    continue;
            }
            // If loginRole = -1 => Account locked
            else if (loginRole == -1)
            {
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
            else
            {

                mIndex = searchMemberByIdInM(&members, studentID);

                vIndex = getViolationIndexById(&violations, studentID);
                menuRole = (members.data[mIndex].isPending == PENDING) ? 0 : loginRole;
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
        switch (menuRole)
        {
        case 0:
        {	
            printf("%s", UI_TABLE_HEADER);
            printf("\nMEMBER MENU\n");
            printf("%s", UI_RESET);
            printf(
                "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                "┃  1. View Profile                             ┃\n"
                "┃  2. View Violation History                   ┃\n"
                "┃  3. View Total Unpaid Fines                  ┃\n"
                "┃  4. View Club Member List                    ┃\n"
                "┃  5. Reset Your Password                      ┃\n"
                "┃  6. View notification                        ┃\n"
                "┃  7. Log Out                                  ┃\n"
                "┃  8. Exit                                     ┃\n");
            if (members.data[mIndex].role > 0){ printf(
                "┃  9. Switch to Admin Menu                     ┃\n");}    
             printf(
                "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");

            inputIntegerInRange(&choice, 1, 9,
                                "==> Enter your selection: ");

            clearScreen();
            switch (choice)
            {
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
                    displayMemberInSort(&members, 0);
                    break;
                case 5:
                    changePassword(&accounts, studentID, menuRole);
                    continue;
                case 6:
                    displayNotificationByMemberID(studentID, ADMIN_WARNING);
                    // displayGlobalNotification();
                    break;
                case 7:
                    isStayLogin = 0;
                    loginRole = -1;
                    menuRole = -1;
                    // mark as not login, reset menu role
                    continue; 
                case 8:
                    isRunning = 0;
                    break;       
                case 9: {
                    if ((loginRole == 1 || loginRole == 2)) {
                        if (members.data[mIndex].isPending == PENDING)
                        {
                            uiError("Pending account. Can only use member menu now. ");
                            break;
                        }
                        else if (members.data[mIndex].isPending == NOT_PENDING)
                        {
                            menuRole = 1;
                            continue;
                        }
                    }
                    else {
                        uiError("Permission denied. Try again.\n");
                        break;
                    }
                }
                default:
                    uiError("Invalid option, please try again.");
            }
            break;
        }
        case 1:
        case 2:
        {
            printf("%s", UI_TABLE_HEADER);
            printf("\n%-48s  %-48s  %-48s\n", " MEMBER", "VIOLATION / REPORT", "NOTIFICATION");
            printf("%s", UI_RESET);

            printf(
                "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                "┃  1. Add New Member                           ┃  ┃  8. Record new violation                     ┃  ┃ 16. Add new notification                     ┃\n"
                "┃  2. Edit Member Information                  ┃  ┃  9. Mark Fine as Paid                        ┃  ┃ 17. Update notification                      ┃\n"
                "┃  3. Remove Member                            ┃  ┃ 10. View Violation List                      ┃  ┃ 18. Delete notification                      ┃\n"
                "┃  4. Check warning/kick list                  ┃  ┃ 11. Statistics by Department                 ┃  ┃ 19. Show all notifications                   ┃\n"
                "┃  5. View Member in Sorted List               ┃  ┃ 12. View Violations by Time Range            ┃  ┃                                              ┃\n"
                "┃  6. Change Member's Password                 ┃  ┃ 13. Delete Violation                         ┃  ┃                                              ┃\n"
                "┃  7. Unlock Member Account                    ┃  ┃ 14. Change Violations Filter/Sort Settings   ┃  ┃                                              ┃\n"
                "┃                                              ┃  ┃ 15. Export Violation Report                  ┃  ┃                                              ┃\n"
                "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");

            printf("%s", UI_TABLE_HEADER);
            printf("\n%-48s\n", " SYSTEM");
            printf("%s", UI_RESET);
            printf(
                "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                "┃ 20. Log Out                                  ┃\n"
                "┃ 21. Exit                                     ┃\n"
                "┃ 22. Switch to Member Menu                    ┃\n"
                "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
            inputIntegerInRange(&choice, 1, 22,
                                " ==> Enter your selection: ");

            clearScreen();
            switch (choice)
            {
            case 1:
                addMember(&members, &accounts, studentID);
                break;
            case 2:
                updateMember(&accounts,&members, &violations, studentID);
                break;
            case 3:
                removeMember(&members, &accounts, &violations, studentID);
                break;
            case 4:
                checkAndWarnOutClub(&members, &accounts, &violations, studentID);
                break;
            case 5:
                displayMemberInSort(&members, 1);
                break;
            case 6:
                changePassword(&accounts, studentID, menuRole);
                break;
            case 7:
                handleLockedAccount(&accounts);
                break;
            case 8:
                recordViolationView(&violations, &members, mIndex);
                break;
            case 9:
                markFineAsPaidView(&violations, &members);
                break;
            case 10:
                flexibleDisplayViolationList(&violations, &members);
                break;
            case 11:
                showFineStatsByTeam(&members, &violations);
                break;
            case 12:
                displayViolationsByTimeRange(&violations);
                break;
            case 13:
                deleteViolation(&violations);
                break;
            case 14:
                changeFilterOption();
                break;
            case 15:
                exportViolationReportToFile(&members, &violations);
                break;
            case 16:
            {
                createNotificationView(&members);
                break;
            }
            case 17:
            {
                updateNotificationView();
                break;
            }
            case 18:
            {
                deleteNotificationView();
                break;
            }
            case 19:
                displayNotificationList();
                break;
            case 20:
                isStayLogin = 0;
                loginRole = -1;
                menuRole = -1;
                continue;
                // mark as not login, reset menu role
            case 21:
                isRunning = 0;
                break;
            case 22:
                menuRole = 0;
                continue;
            default:
                uiError("Invalid option, please try again.\n");
                break;
            }
        }
        }
        pauseProgram();
    } while (isRunning);

    saveAccounts(&accounts);
    saveMembers(&members);
    saveViolations(&violations);

    if (violations.data != NULL)
    {
        free(violations.data);
    }
    freeNotificationList();
    return 0;
}

void seedSampleData(MemberList *members, ViolationList *violations, AccountList *accounts)
{
    // =============================================================
    // 3 MEMBERS
    // =============================================================

    // --- Member 0: Regular member, Academic, no violations ---
    strcpy(members->data[0].fullName, "Nguyen Van An");
    strcpy(members->data[0].email, "anNV@fpt.edu.vn");
    strcpy(members->data[0].phoneNumber, "0901234561");
    strcpy(members->data[0].studentID, "SE200001");
    members->data[0].team = 0; // Academic
    members->data[0].role = 0; // Member -> fine = 20,000
    members->data[0].violationCount = 2;
    members->data[0].consecutiveAbsences = 1;
    members->data[0].totalFine = 40000.0;
    members->data[0].isPending = NOT_PENDING;

    // --- Member 1: Leader, HR, absent 3 times in a row -> pending kick ---
    strcpy(members->data[1].fullName, "Tran Thi Bich");
    strcpy(members->data[1].email, "bichTT@fpt.edu.vn");
    strcpy(members->data[1].phoneNumber, "0901234562");
    strcpy(members->data[1].studentID, "SE200002");
    members->data[1].team = 2; // HR
    members->data[1].role = 1; // Leader/Vice -> fine = 50,000
    members->data[1].violationCount = 3;
    members->data[1].consecutiveAbsences = 2;
    members->data[1].totalFine = 150000.0;
    members->data[1].isPending = NOT_PENDING;

    // --- Member 2: BCN, Planning, warning ---
    strcpy(members->data[2].fullName, "Le Hoang Cuong");
    strcpy(members->data[2].email, "cuongLH@fpt.edu.vn");
    strcpy(members->data[2].phoneNumber, "0901234563");
    strcpy(members->data[2].studentID, "SE200003");
    members->data[2].team = 1; // Planning
    members->data[2].role = 2; // BCN -> fine = 50,000
    members->data[2].violationCount = 4;
    members->data[2].consecutiveAbsences = 1;
    members->data[2].totalFine = 200000.0;
    members->data[2].isPending = NOT_PENDING;

    members->count = 3;

    // =============================================================
    // 9 VIOLATIONS (3 per member, mix paid/unpaid)
    // =============================================================
    time_t base = 1745020800; // 2026-04-19 00:00 UTC

    // SE200001
    for (int i = 0; i < 2; i++)
    {
        strcpy(violations->data[i].studentID, members->data[0].studentID);
        violations->data[i].fine = 20000.0;
        violations->data[i].penalty = 0;
        violations->data[i].owner = &members->data[0];
        violations->data[i].owner->isPending = NOT_PENDING;
    }
    strcpy(violations->data[0].violationID, "VIO001");
    violations->data[0].reason = REASON_MEETING_ABSENCE;
    violations->data[0].isPaid = 0;
    strcpy(violations->data[0].note, "Retrospective meeting");

    strcpy(violations->data[1].violationID, "VIO002");
    violations->data[1].reason = REASON_NOT_UNIFORM;
    violations->data[1].isPaid = 0;
    strcpy(violations->data[1].note, "");

    // SE200002
    for (int i = 2; i < 5; i++)
    {
        strcpy(violations->data[i].studentID, members->data[1].studentID);
        violations->data[i].penalty = PENALTY_FINANCIAL;
        violations->data[i].owner = &members->data[1];
    }

    strcpy(violations->data[2].violationID, "VIO003");
    violations->data[2].reason = REASON_MEETING_ABSENCE;
    violations->data[2].fine = 50000.0;
    strcpy(violations->data[2].note, "Retrospective meeting");
    violations->data[2].isPaid = ALREADY_PAID;

    strcpy(violations->data[3].violationID, "VIO004");
    violations->data[3].reason = REASON_MEETING_ABSENCE;
    violations->data[3].fine = 50000.0;
    strcpy(violations->data[3].note, "SUMMER semester meeting");
    violations->data[3].isPaid = ALREADY_PAID;

    strcpy(violations->data[4].violationID, "VIO005");
    violations->data[4].reason = REASON_MEETING_ABSENCE;
    strcpy(violations->data[4].note, "Daily report meeting");
    violations->data[4].penalty = PENALTY_KICK;
    violations->data[4].fine = 0;
    violations->data[4].isPaid = NOT_HAVE_TO_PAY;

    // SE200003
    for (int i = 5; i < 9; i++)
    {
        strcpy(violations->data[i].studentID, members->data[2].studentID);
        violations->data[i].fine = 50000.0;
        violations->data[i].penalty = 0;
        violations->data[i].owner = &members->data[2];
        violations->data[i].owner->isPending = NOT_PENDING;
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

    for (int i = 0; i <= 8; i++)
    {
        violations->data[i].violationTime = base;
    }

    // =============================================================
    // 3 ACCOUNTS (default password = studentID)
    // =============================================================

    strcpy(accounts->data[0].studentID, "SE200001");
    strcpy(accounts->data[0].password, "SE200001");
    accounts->data[0].role = 0; // Member
    accounts->data[0].isLocked = 0;
    accounts->data[0].failCount = 0;

    strcpy(accounts->data[1].studentID, "SE200002");
    strcpy(accounts->data[1].password, "SE200002");
    accounts->data[1].role = 1; // Vice or Leader
    accounts->data[1].isLocked = 0;
    accounts->data[1].failCount = 0;

    strcpy(accounts->data[2].studentID, "SE200003");
    strcpy(accounts->data[2].password, "SE200003");
    accounts->data[2].role = 2; // Admin/BOD
    accounts->data[2].isLocked = 0;
    accounts->data[2].failCount = 0;

    accounts->count = 3;
    saveMembers(members);
    saveAccounts(accounts);
    saveViolations(violations);

    printf("Created sample data with Pending status in data/ folder.\n");
}