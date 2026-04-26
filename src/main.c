#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../include/view/viewUtil.h"
#include "../include/utils.h"
#include "../include/member.h"
#include "../include/auth.h"
#include "../include/violation.h"
#include "../include/validate.h"
#include "../include/consoleInput.h"
#include "../sampleData/sampleData.h"
#include "../include/view/violationView.h"

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	
	SetConsoleOutputCP(65001);
	int mCount = 1000, vCount = 1000, aCount = 1000; //set to max, then reset later
	char studentID[10];
	
	// set the array to be empty.
	Member members[MAX_MEMBERS] = {0};
	Violation violations[MAX_VIOLATIONS] = {0};
	Account accounts[MAX_ACCOUNTS] = {0};
	
	seedSampleData(members, &mCount , violations, &vCount, accounts, &aCount);
	
	mCount = loadMembers(members, &mCount);
	vCount = loadViolations(violations, &vCount);
	aCount = loadAccounts(accounts, &aCount);
	
	// loginRole represent the role of this account
	// menuRole represent which menu will be open.
	
	int loginRole = -1, menuRole = -1;
	int choice = -1, isStayLogin = 0;
	// isStayedLogin check whether user is auth-ed. 0 = No, 1 = Yes;
	int mIndex = -1, vIndex = -1;
	
	do {
		// Firstly, auth. If stay login ==> do not check
		//				  If not stay login =>> check again.
		if (isStayLogin == 0){
			loginRole = login(accounts, studentID, aCount);
			if (loginRole == -1){
				int isExit = 0;
				inputYesNo(&isExit, "Student ID does not exist, do you want to exit? \n 1. Yes\n 0. No\n");
				if (isExit == 1) return 0;
				else continue;	
			} 
			else {
				menuRole = loginRole;
				mIndex = searchMemberByIdInM(members, sizeof(members), studentID);
				vIndex = searchMemberByIdInV(violations, sizeof(violations), studentID);
				isStayLogin = 1;
			// login successfully ==> Assign value for menuRole to open menu, and memberIndex to identify user
			}
		}
		
		system(CLEAR);
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
					"\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
				);

				inputIntegerInRage(&choice, 1, 8, "==> Enter your selection: ");
					
					system(CLEAR);
					switch (choice){
						case 1: 
							displayOneMemberInfo(members[mIndex]);
							break;
						case 2: 
							displayViolationByStudentId(studentID, violations, vCount);
							break;
						case 3: 
							// not finish yet
							break;
						case 4: 
							displayMemberList(members, mCount);
							break;
						case 5: 
							changePassword(accounts, aCount);
							break;
						case 6: 
							isStayLogin = 0;
							loginRole = -1;
							menuRole = -1;
							// mark as not login, reset menu role
							break;
						case 7: 
							// if choice = 7, save the data end return.
							return 0;
						case 8: 
							if (loginRole == 1 || loginRole == 2){
								menuRole = 1;
							} else {
								printf("Permission denied. Try again.");
							}
							break;
						default:
							printf("Invalid option, please try again.");	
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
						"\n┃  4.  Record Violation                        ┃"
						"\n┃  5.  Mark Fine as Paid                       ┃"
						"\n┃  6.  View Violation List                     ┃"
						"\n┃  7.  Statistics by Department                ┃"
						"\n┃  8.  Reset Member's Password                 ┃"
						"\n┃  9.  Log Out                                 ┃"
						"\n┃ 10.  Exit                                    ┃"
						"\n┃ 11.  Switch to Member Menu                   ┃"
						"\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
					);
					inputIntegerInRage(&choice, 1, 11, "│ ==> Enter your selection: ");

					system(CLEAR);
					switch (choice){
						case 1: 
							addMember(members, &mCount, accounts, &aCount);
							break;
						case 2: 
							updateMember(members, &mCount, violations, vCount);
							break;
						case 3: 
							removeMember(members, &mCount, accounts, &aCount, violations, &vCount);
							break;
						case 4: 
							// wait for code addViolations
							break;
						case 5: 
							// wait for code markAsPaid
							break;
						case 6: 
							displayViolationList(violations, vCount);
							// not sorted by team, role yet
							break;
						case 7: 
							showTotalFineByRole(members, mCount);
							break;
						case 8:
							// wait for code statistic totalFine by team
							break;
						case 9:
							isStayLogin = 0;
							loginRole = -1;
							menuRole = -1;
							// mark as not login, reset menu role
							break;	
						case 10: 
							return 0;
						case 11: 
							menuRole = 0;
							// change menuRole ==> Open personal menu instead of admin menu
							break;
						default:
							printf("Invalid option, please try again.");
					}
				break;
			}
			
			
		}
		system("pause");
	} while (1);	
	return 0;
}


