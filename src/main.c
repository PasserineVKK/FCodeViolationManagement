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

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	
	SetConsoleOutputCP(65001);
	int mCount = 1000, vCount = 1000, aCount = 1000; //set to max, then reset later
	char studentID[10];
	
	// set the array to be empty.
	Member members[MAX_MEMBERS] = {0};
	Violation violations[MAX_VIOLATIONS] = {0};
	Account accounts[MAX_ACCOUNTS] = {0};
	
	//seedSampleData(members, &mCount , violations, &vCount, accounts, &aCount);
	//------use above command if you've not created seed data
	
	mCount = loadMembers(members, &mCount);
	vCount = loadViolations(violations, &vCount);
	aCount = loadAccounts(accounts, &aCount);
	
	// loginRole represent the role of this account
	// menuRole represent which menu will be open.
	
	int loginRole = -1, menuRole = -1;
	int choice = -1, isStayLogin = 0;
	// isStayedLogin check whether user is auth-ed. 0 = No, 1 = Yes;
	int memberIndex = -1;
	
	do {
		// Firstly, auth. If stay login ==> do not check
		//				  If not stay login =>> check again.
		if (isStayLogin == 0){
			loginRole = login(accounts, studentID, aCount);
			if (loginRole == -1) return 0;
			// login failed ==> Program stop
			else {
				menuRole = loginRole;
				memberIndex = searchMemberByIdInM(members, sizeof(members), studentID);
				isStayLogin = 1;
			// login successfully ==> Assign value for menuRole to open menu, and memberIndex to identify user
			}
		}
		
		
		switch (menuRole) {
			case 0: {
				
					inputPosInteger(&choice, 
					    "\n  =========================================="
					    "\n  ¦              MEMBER MENU               ¦"
					    "\n  =========================================="
					    "\n  ¦  1. View Profile                       ¦"
					    "\n  ¦  2. View Violation History             ¦"
					    "\n  ¦  3. View Total Unpaid Fines            ¦"
					    "\n  ¦  4. View Club Member List              ¦"
					    "\n  ¦  5. Reset Password                     ¦"
					    "\n  ¦  6. Log Out                            ¦"
					    "\n  ¦  7. Exit                               ¦"
					    "\n  ¦  8. Switch to Admin Menu (Admin Only)  ¦"
					    "\n  =========================================="
					    "\n  ==> Enter your selection: "
					);
					
					switch (choice){
						case 1: 
							displayOneMemberInfo(members[memberIndex]);
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
			
			case 1:{
					inputPosInteger(&choice, 
					    "\n  =========================================="
					    "\n  ¦               ADMIN MENU               ¦"
					    "\n  =========================================="
					    "\n  ¦  1. Add New Member                     ¦"
					    "\n  ¦  2. Edit Member Information            ¦"
					    "\n  ¦  3. Remove Member                      ¦"
					    "\n  ¦  4. Record Violation                   ¦"
					    "\n  ¦  5. Mark Fine as Paid                  ¦"
					    "\n  ¦  6. View Violation List                ¦"
					    "\n  ¦  7. Statistics by Department           ¦"
					    "\n  ¦  8. Reset Member's Password            ¦"
					    "\n  ¦  9. Reset Own Password                 ¦"
					    "\n  ¦  10. Log Out                           ¦"
					    "\n  ¦  11. Exit                              ¦"
					    "\n  ¦  12. Switch to Personal Member Menu    ¦"				    				    
					    "\n  =========================================="
					    "\n  ==> Enter your selection: "
					);
					
					switch (choice){
						case 1: 
							//add mem
							break;
						case 2: 
							//edit mem
							break;
						case 3: 
							//del mem
							break;
						case 4: 
							//add vio
							break;
						case 5: 
							//mark paid
							break;
						case 6: 
							//view vio
							break;
						case 7: 
							//statistic
							break;
						case 8:
							// reset pass
							break;
						case 9: 
							// reset pass
							break;
						case 10:
							isStayLogin = 0;
							loginRole = -1;
							menuRole = -1;
							// mark as not login, reset menu role
							break;	
						case 11: 
							return 0;
						case 12: 
							menuRole = 0;
							// change menuRole ==> Open personal menu instead of admin menu
							break;
						default:
							printf("Invalid option, please try again.");
					}
				break;
			}
			
			case 2:
				break; 
		}
	} while (1);	
	return 0;
}


