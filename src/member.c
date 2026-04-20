#include <stdio.h>
#include <string.h>
#include "../include/member.h"
#include "../include/violation.h"
#include "../include/fileio.h"

//file
int loadMembers(Member members[], int *count) {
    return loadFromFile(MEMBERS_FILE, members, sizeof(Member), MAX_MEMBERS, count);
}

int saveMembers(Member members[], int count) {
    return saveToFile(MEMBERS_FILE, members, sizeof(Member), count);
}

//search
int searchMemberById(Member members[], int count, const char *id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(members[i].studentID, id) == 0) {
            return i;
        }
    }
    return -1;
}

displayOneMemberInfo (Member member) {
    printf("\n┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━┳━━━━━━┳━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━┓\n");
    printf("┃ %-11s ┃ %-20s ┃ %-25s ┃ %-12s ┃ %-5s ┃ %-5s ┃ %-15s ┃ %-20s ┃ %-11s ┃ %-10s ┃\n",
        "Student ID", "Name", "Email", "Phone", "Team", "Role", "Violation Count", "Consecutive Absences", "Total Fine", "Status");
    printf("┣━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━┫\n");

    printf("┃ %-11s ┃ %-20s ┃ %-25s ┃ %-12s ┃ %-5d ┃ %-5d ┃ %-15d ┃ %-20d ┃ %-11.2f ┃ %-10d ┃\n",
        member.studentID,
        member.fullName,
        member.email,
        member.phoneNumber,
        member.team, 
        member.role, 
        member.violationCount,
        member.consecutiveAbsences,
        member.totalFine,
        member.status);
    printf("┗━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━┻━━━━━━┻━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━┛\n");
}

//update: update tổng tiền nợ cho thành viên
int updateMemberTotalFine(const char *id) {
    Member members[MAX_MEMBERS];
    int mCount = 0;
    if(loadMembers(members, &mCount) == -1 && mCount == 0) {
        printf("Error loading members data.\n");
        return 0;
    }
    int mIndex = searchMemberById(members, mCount, id);
    if(mIndex == -1) {
        printf("Member with ID %s not found.\n", id);
        return 0;
    }
    //tải vi pham của thành viên
    Violation violations[MAX_VIOLATIONS];
    int vCount = 0;
    loadViolations(violations, &vCount);
    //tính tổng tiền nợ
    //chỉ tính những vi phạm chưa được thanh toán và không đang chờ xử lý
    double totalFine = 0.0;
    for(int i = 0; i < vCount; i++) {
        if(strcmp(violations[i].studentID, id) == 0 
        && violations[i].isPaid == 0
        && violations[i].isPending == 0) {
            totalFine += violations[i].fine;
        }
    }
    members[mIndex].totalFine = totalFine;
    saveMembers(members, mCount);
    return 1;
}


// ===== Feature 2.1: ADD MEMBER =====
void addMember(Member members[], int *count) {
    if (*count >= MAX_MEMBERS) {
        printf("Member list is full!\n");
        return;
    }
	if (*count >= 1000) {
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
		inputMemberTeam(team, "Enter team (0-3): ");

		//Input role info
		inputMemberRole(role, "Enter role (0-2): ");

		//Confirm to add member
		int confirm;
		do {
			inputYesNo(&confirm,
			         "\nAdd this member?\n1: Yes\n0: No\n=> Your choice: ");
		
		} while (!isOneOrZero(confirm));

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
			mem.status = 1;

			// add member to member list
			members[(*count)++] = mem;

			//Call save member to file function
			saveMembers(members, *count);

			//Print success message
			printf("Member added successfully!\n");

			//*************************Give back account info to user

		} else {
			printf("Member not added.\n");
		}

		// ===== CONTINUE =====
		int choice;
		do {
			inputYesNo(&choice,
			         "\nDo you want to add another member?\n1: Yes\n0: No\n=> Your choice: ");
		
		} while (!isOneOrZero(choice));

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
void removeMember(Member members[], int *count) {
	//Check if member list is empty
	if (*count == 0) {
		printf("No members available to remove.\n");
		return;
	}

	char id[9]; 
	int pos;

	int  continueRemove = 1;
	while (continueRemove) {
		pos = -1; //Reset position before find member
		inputStudentID(id, "Nhap studentID can xoa: ");

		//Find member by ID and remove by shift left array
		pos = searchMemberById(members, *count, id);
    

		if (pos != -1) {
			// Show student
			printf("\nStudent found:\n");
			displayOneMemberInfo(members[pos]);

			//Confirm to remove member
			int confirm;
			do {
				inputYesNo(&confirm,
						"\nRemove this member?\n1: Yes\n0: No\n=> Your choice: ");
			
			} while (!isOneOrZero(confirm));

			//Start to remove
			if (confirm == 1) {

				for (int i = pos; i < *count - 1; i++){
					//Shift left array
					members[i] = members[i + 1];
				}
				//Decrease member size
				(*count)--;

				//Call save member to file function
				saveMembers(members, *count);

				//Print success message
				printf("Member removed successfully!\n");

				//****************Xóa thêm thông tin account và violation liên quan đến member này 

			} else {
				printf("Member not removed.\n");
			}
		}
		else {
			printf("Member not found!\n");
		}


		// ===== CONTINUE =====
		int choice;
		do {
			inputYesNo(&choice,
			         "\nDo you want to remove another member?\n1: Yes\n0: No\n=> Your choice: ");
		
		} while (!isOneOrZero(choice));

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

	int pos;

	int  continueUpdate = 1;
	while (continueUpdate) {
		pos = -1; //Reset position before find member
		inputStudentID(studentID, "Nhap studentID can cap nhat: ");

		//Find member by ID and update by assign new value to target member
		pos = searchMemberById(members, *mCount, studentID);

        //If found 
		if (pos != -1) {
			// Show student
			printf("\nStudent found:\n");
			displayOneMemberInfo(members[pos]);
            
			//Ask which field want to update
			int fieldChoice;
			do {
				printf("\nWhich field do you want to update?\n");
				printf("1: Name\n2: Email\n3: Phone Number\n4: Team\n5: Role\n=> Your choice: ");
				inputPosInteger(&fieldChoice, "=> Your choice (only 1-5): ");

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
			do {
				inputYesNo(&confirm,
						"\nUpdate this member?\n1: Yes\n0: No\n=> Your choice: ");
			
			} while (!isOneOrZero(confirm));

            if (confirm == 1) {
                //Call save member to file function
                switch (fieldChoice) {
                    case 1:
                        strcpy(members[pos].fullName, fullName);
                        break;

                    case 2:	
                        strcpy(members[pos].email, email);
                        break;
                    
                    case 3:
                        strcpy(members[pos].phoneNumber, phoneNumber);
                        break;

                    case 4:
                        members[pos].team = team; //Assign new team
                        break;
                        
                    case 5:
                        int oldRole = members[pos].role; //Save old role before assign new role
                        members[pos].role = role; //Assign new role
                                            
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
                            members[pos].totalFine = updateMemberTotalFine(studentID); 
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
                            members[pos].totalFine = updateMemberTotalFine(studentID); 
                        }

                        break;	
                }

            }
        }
    
        else {
            printf("Member not found!\n");
        }   
        
        // ===== CONTINUE =====
		int choice;
		do {
			inputYesNo(&choice,
			         "\nDo you want to update another member?\n1: Yes\n0: No\n=> Your choice: ");
		
		} while (!isOneOrZero(choice));

		if (choice == 0) {
			continueUpdate = 0;
		}
    }    			
}


