#include <stdio.h>
#include <string.h>
#include "../include/member.h"
#include "../include/auth.h"
#include "../include/violation.h"
#include "../include/validate.h"
#include "../include/fileio.h"
#include "../include/consoleInput.h"
#include "../include/utils.h"

Member memberList[MAX_MEMBERS];

//file
int loadMembers(Member members[], int *count) {
    return loadFromFile(MEMBERS_FILE, members, sizeof(Member), MAX_MEMBERS, count);
}

int saveMembers(Member members[], int count) {
    return saveToFile(MEMBERS_FILE, members, sizeof(Member), count);
}

//search
int searchMemberByIdInM(Member members[], int count, const char *id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(members[i].studentID, id) == 0) {
            return i;
        }
    }
    return -1;
}

void displayOneMemberInfo(Member member)
{
    printf("\n┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━┳━━━━━━┳━━━━━━━━━━┓\n");

    printf("┃ %-10s ┃ %-24s ┃ %-28s ┃ %-12s ┃ %-4s ┃ %-4s ┃ %-8s ┃\n",
           "Student ID",
           "Name",
           "Email",
           "Phone",
           "Team",
           "Role",
           "Status");

    printf("┣━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━━━━━┫\n");

    printf("┃ %-10s ┃ %-24s ┃ %-28s ┃ %-12s ┃ %-4d ┃ %-4d ┃ %-8s ┃\n",
           member.studentID,
           member.fullName,
           member.email,
           member.phoneNumber,
           member.team,
           member.role,
           member.isPending ? "Pending" : "Active");

    printf("┗━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━┻━━━━━━┻━━━━━━━━━━┛\n");
}
// Count unpaid violations for a member
int  countUnpaidViolations(const char *id, Violation violations[], int vCount) {
	int unpaidCount = 0;
	for (int i = 0; i < vCount; i++) {
		if(strcmp(violations[i].studentID, id) == 0 
        && violations[i].isPaid == 0
        && violations[i].isPending == 0) {
            unpaidCount++;
        }
	}
	return unpaidCount;

}

//updateTotalFine

int updateMemberTotalFine(Member members[], int mCount, Violation violations[], int vCount, const char *id) {
    int memberIndex = searchMemberByIdInM(members, mCount, id);
	if (memberIndex == -1) {
		printf("Member not found!\n");
		return 0; 
	}
	double totalFine = 0;
	for (int i = 0; i < vCount; i++) {
		if (strcmp(violations[i].studentID, id) == 0 
		&& violations[i].isPaid == 0
		&& violations[i].isPending == 0) {
			totalFine += violations[i].fine;
		}
	}
	members[memberIndex].totalFine = totalFine;
	return 1;

}

int updateConsecutiveAbsences(Member members[], int count, const char *id) {
    int index = searchMemberByIdInM(members, count, id);

    if (index != -1) {
        members[index].consecutiveAbsences++;
     	//-> function trigger warning if absence = 2 may be insert here
     	return 1;
    } else {
        return 0;
    }
}

//1.4: View member list
void displayMemberList(Member members[], int count) {
	printf("\n");
    printf("┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃ %-10s ┃ %-22s ┃ %-10s ┃ %-18s ┃\n",
           "Student ID", "Full Name", "Team", "Role");
    printf("┣━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━┫\n");
	for (int i = 0; i < count; i++) {
        printf("┃ %-10s ┃ %-22s ┃ %-10s ┃ %-18s ┃\n",
               members[i].studentID,
               members[i].fullName,
               translateTeam(members[i].team),
               translateRole(members[i].role));
    }
	printf("┗━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━┛\n");
    printf("Total members: %d\n\n", count);
	
}


// ===== Feature 2.1: ADD MEMBER =====
void addMember(Member members[], int *mCount, Account accounts[], int *aCount) {
    if (*mCount >= MAX_MEMBERS) {
        printf("Member list is full!\n");
        return;
    }

	char fullName[50];
	char email[50];
	char phoneNumber[11];
	char studentID[9]; //SE000000
	int team; // 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
	int role;
    
	int continueAdd = 1;

	//Use loop to let user add multiple member until they choose to stop
	while (continueAdd) {
		//Input member name
		inputMemberName(fullName, "Enter full name: ");

		//Input member email
		inputMemberEmail(email, "Enter email: ");

		//Input student ID
		inputStudentID(studentID, "Enter student ID: ");

		//Input member phone number
		inputMemberPhone(phoneNumber, "Enter phone number: ");
		
		//Input team info
		inputMemberTeam(&team, "Enter team (0-3): ");	

		//Input role info
		inputMemberRole(&role, "Enter role (0-2): ");

		//Confirm to add member
		int confirm;
		
		inputYesNo(&confirm, "\nAdd this member?\n1: Yes\n0: No\n=> Your choice: ");
		

		if (confirm == 1) {

			//Ceate member struct and assign value
			Member mem;

			strcpy(mem.fullName, fullName);
			strcpy(mem.studentID, studentID);
			strcpy(mem.email, email);
			strcpy(mem.phoneNumber, phoneNumber);
			mem.team = team;
			mem.role = role;
			mem.violationCount = 0;
			mem.consecutiveAbsences = 0;
			mem.totalFine = 0;
			mem.isPending = 0;

			//Create account for this member with default password "123456"
			Account acc;

			strcpy(acc.studentID, studentID);
			strcpy(acc.password, studentID); //Default password is student ID
			acc.role = role;
			acc.isLocked = 0;
			acc.failCount = 0;

			// add member to member list
			members[(*mCount)++] = mem;
			// add account to account list
			accounts[(*aCount)++] = acc;

			//Call save member to file function
			saveMembers(members, *mCount);
			//Call save account to file function
			saveAccounts(accounts, *aCount); 

			//Print success message
			printf("Member added successfully!\n");

			//Give back account info to user
			printf("\nAccount information for this member:\n");
			printf("Student ID: %s\n", acc.studentID);
			printf("Password: %s\n", acc.password);


		} else {
			printf("Member not added.\n");
		}

		// ===== CONTINUE =====
		int choice;
		inputYesNo(&choice,"\nDo you want to add another member?\n1: Yes\n0: No\n=> Your choice: ");
		
		if (choice == 0) {
			continueAdd = 0;
		}
	}
}

// ===== Feature 2.2: REMOVE MEMBER =====
/*
Input ID => Find by ID => If found, show member info 
=> Confirm to remove => If yes, remove by shift left array => Save to file
*/
void removeMember(Member members[], int *mCount, Account accounts[], int *aCount, Violation violations[], int *vCount) {
	//Check if member list is empty
	if (*mCount == 0) {
		printf("No members available to remove.\n");
		return;
	}

	char id[9]; 
	int pos;

	int  continueRemove = 1;
	while (continueRemove) {
		int mIndex = -1, vIndex = -1, aIndex = -1; //Reset position before find member
		inputStudentID(id, "Enter student ID to remove: ");

		//Find member by ID in member list
		mIndex = searchMemberByIdInM(members, *mCount, id);
    

		if (mIndex != -1) {
			// Show student
			printf("\nStudent found:\n");
			displayOneMemberInfo(members[mIndex]);

			//Confirm to remove member
			int confirm;
			inputYesNo(&confirm, "\nRemove this member?\n1: Yes\n0: No\n=> Your choice: ");
			
			//Start to remove
			if (confirm == 1) {

				// Search member in violation list and account list 
				vIndex = searchMemberByIdInV(violations, *vCount, id);
		 		aIndex = searchMemberByIdInA(accounts, *aCount, id);

				// Remove in member list by shift left array
				for (int i = mIndex; i < *mCount - 1; i++){
					//Shift left array
					members[i] = members[i + 1];
				}

				// Remove in account list by shift left array
				for (int i = aIndex; i < *aCount - 1; i++){
					//Shift left array
					accounts[i] = accounts[i + 1];
				}

				//Decrease member size
				(*mCount)--;
				//Decrease account size
				(*aCount)--;

				//Call save member to file function
				saveMembers(members, *mCount);
				//Call save account to file function
				saveAccounts(accounts, *aCount);

				// Remove in violation list by shift left array if found
				// If vIndex == -1 => This member has no violation record 
				// => No need to remove in violation list
				if (vIndex != -1) {
					for (int i = vIndex; i < *vCount - 1; i++){
						//Shift left array
						violations[i] = violations[i + 1];
					}
					(*vCount)--;
					saveViolations(violations, *vCount);
				}

				//Print success message
				printf("Member removed successfully!\n");

			} else {
				printf("Member not removed.\n");
			}
		}
		else {
			printf("Member not found!\n");
		}


		// ===== CONTINUE =====
		int choice;
		
		inputYesNo(&choice, "\nDo you want to remove another member?\n1: Yes\n0: No\n=> Your choice: ");

		if (choice == 0) {
			continueRemove = 0;
		}

	}
	
}

// ===== Feature 2.3: UPDATE MEMBER =====
/*
Input ID => Find by ID => If found, show member info
=> ask which field want to update 
=> input new value => confirm to update 
=> If yes, update by assign new value to target member 
=> Save to file
*/
void updateMember(Member members[], int *mCount, Violation violations[], int vCount) {
	//Check if member list is empty
	if (*mCount == 0) {
		printf("No members available to update.\n");
		return;
	}

	char fullName[50];
	char email[50];
	char phoneNumber[11];
	char studentID[9]; //SE000000
	int team; // 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
	int role; // 0 = Member, 1 = Leader/Vice, 2 = BOD

	int mIndex;

	int  continueUpdate = 1;
	while (continueUpdate) {
		mIndex = -1; //Reset position before find member
		inputStudentID(studentID, "Enter student ID to update: ");

		//Find member by ID and update by assign new value to target member
		mIndex = searchMemberByIdInM(members, *mCount, studentID);

        //If found 
		if (mIndex != -1) {
			// Show student
			printf("\nStudent found:\n");
			displayOneMemberInfo(members[mIndex]);
            
			//Ask which field want to update
			int fieldChoice;
			do {
				printf("\nWhich field do you want to update?\n");
				printf("1: Name\n2: Email\n3: Phone Number\n4: Team\n5: Role\n=> Your choice: ");
				inputIntegerInRage(&fieldChoice, 1, 5, "=> Your choice (only 1-5): ");
				while (getchar() != '\n');

			} while (fieldChoice < 1 || fieldChoice > 5);

			switch (fieldChoice) {
				case 1:
					//Input new name
					inputMemberName(fullName, "Enter New Name: ");
					break;

				case 2:	
					//Input new email
					inputMemberEmail(email, "Enter New Email: ");
					break;
				
				case 3:
					//Input new phone number
					inputMemberPhone(phoneNumber, "Enter New Phone Number: ");
					break;

				case 4:
					//Input new team info
	 				inputMemberTeam(&team, "Enter New Team (0-3): ");
					break;
					
				case 5:
					//Input new role info
					inputMemberRole(&role, "Enter New Role (0-2): ");
					break;	
			}
            
            //Confirm to update member
            int confirm;
			inputYesNo(&confirm, "\nUpdate this member?\n1: Yes\n0: No\n=> Your choice: ");
			while (getchar() != '\n');

            if (confirm == 1) {
                switch (fieldChoice) {
                    case 1:
                        strcpy(members[mIndex].fullName, fullName);
                        break;

                    case 2:	
                        strcpy(members[mIndex].email, email);
                        break;
                    
                    case 3:
                        strcpy(members[mIndex].phoneNumber, phoneNumber);
                        break;

                    case 4:
                        members[mIndex].team = team; 
                        break;
                        

                    case 5:
                        {
							int oldRole = members[mIndex].role; //Save old role before assign new role
							members[mIndex].role = role; //Assign new role
												
							//If member change from Member to Leader/Vice or BCN
							if (oldRole == 0 && role > 0) {
								
								//Replace new fines for all unpaid and not pending violation of this member
								for (int i = 0; i < vCount; i++) {
									if (strcmp(violations[i].studentID, studentID) == 0 
									&& violations[i].isPaid == 0
									&& violations[i].isPending == 0
									&& violations[i].fine != 0) { 
									
										double newFine = calculateFine(role, violations[i].reason);

										//Leader/Vice or BOD have higher fine than Member 30.000 per violation
										violations[i].fine = newFine; 
									}
								}
								
								//Update total fine for this member after change role
								updateMemberTotalFine(members, *mCount, violations, vCount, studentID); 
							}

							//If member change from Leader/Vice or BCN to Member
							else if (oldRole > 0 && role == 0) {
								
								//Replace new fines for all unpaid and not pending violation of this member
								for (int i = 0; i < vCount; i++) {
									if (strcmp(violations[i].studentID, studentID) == 0 
									&& violations[i].isPaid == 0
									&& violations[i].isPending == 0
									&& violations[i].fine != 0) {
									
										double newFine = calculateFine(role, violations[i].reason);
										//Leader/Vice or BOD have higher fine than Member 30.000 per violation
										violations[i].fine = newFine; 
									}
								}
								//Update total fine for this member after change role
								updateMemberTotalFine(members, *mCount, violations, vCount, studentID); 
							}

							break;
						}	
                }

            }
        }
    
        else {
            printf("Member not found!\n");
        }   
        
        // ===== CONTINUE =====
		int choice;
		inputYesNo(&choice, "\nDo you want to update another member?\n1: Yes\n0: No\n=> Your choice: ");
		while (getchar() != '\n');

		if (choice == 0) {
			continueUpdate = 0;
		}
    }    			
}

// ===== Feature 2.7: SHOW TOTAL FINE BY ROLE =====
void showTotalFineByRole(Member members[], int mCount) {
	const char *team[] = {"Academic", "Planning", "HR", "Media"};
	double totalFineByTeam[4] = {0};

	for (int i = 0; i < mCount; i++) {		
		totalFineByTeam[members[i].role] += members[i].totalFine;
	}
	printf("\n┏━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃ %-20s ┃ %-40s ┃\n", "Team", "Total Fine of that team");
    printf("┣━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");

    for (int i = 0; i < 4; i++) {
        printf("┃ %-20s ┃ %-40.2f ┃\n", team[i], totalFineByTeam[i]);
    }

    printf("┗━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}


