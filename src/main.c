#include <stdio.h>
#include <stdlib.h>
#include "../include/view/viewUtil.h"
#include "../include/utils.h"
#include "../include/member.h"
#include "../include/auth.h"
#include "../include/violation.h"
#include "../include/validate.h"
#include "../include/consoleInput.h"
#include "../sampleData/sampleData.h"


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	
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
	
	
	int menuRole = -1; 
	int choice = -1;
	menuRole = login(accounts, studentID, aCount);
	
	switch (menuRole) {
		case 0: {
			do {
				inputPosInteger(&choice, 
				    "\n  =========================================="
				    "\n  ¦              MEMBER MENU               ¦"
				    "\n  =========================================="
				    "\n  ¦  1. View Profile                       ¦"
				    "\n  ¦  2. View Violation History             ¦"
				    "\n  ¦  3. View Total Unpaid Fines            ¦"
				    "\n  ¦  4. View Club Member List              ¦"
				    "\n  ¦  5. Reset Password                     ¦"
				    "\n  ¦  6. Log Out and Exit                   ¦"
				    "\n  =========================================="
				    "\n  ==> Enter your selection: "
				);
				
				switch (choice){
					case 1: 
						//view profile
						break;
					case 2: 
						//vio violation
						break;
					case 3: 
						//view unpaid fine
						break;
					case 4: 
						// view club memeber list
						break;
					case 5: 
						//reset
						break;
					case 6: 
						//log out
						break;
					default:
						printf("Invalid option, please try again.");
				}
			} while (choice != 6);
			break;
		}
		
		case 1:{
			do {
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
				    "\n  ¦  10. Log Out and Exit                  ¦"
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
						break;	
					default:
						printf("Invalid option, please try again.");
				}
			} while (choice != 10);
			break;
		}
		
		case 2:
			break; 
	}
	
	return 0;
	
}
