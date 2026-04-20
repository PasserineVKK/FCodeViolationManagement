#include "../include/memberManager.h"
#include "../include/validate.h"
#include "../include/consoleInput.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//Load member from file
void loadMemberFromFile(MemberManager *m) {
	FILE *f = fopen("member.dat", "rb");
	if (f == NULL) {
		printf("Khong co file, tao moi!\n");
		m->size = 0;
		return;
	}

	//Get recent member members size
	fread(&m->size, sizeof(int), 1, f);

	//Real all members, copy data to members
	fread(m->members, sizeof(Member), m->size, f);

	fclose(f);
}

//Save member to file
void saveMemberToFile(MemberManager *m) {
	FILE *f = fopen("member.dat", "wb");
	if (f == NULL) {
		printf("Khong mo duoc file!\n");
		return;
	}

	// Get recent member members size
	fwrite(&m->size, sizeof(int), 1, f);

	//Write all member to file from member list
	fwrite(m->members, sizeof(Member), m->size, f);

	fclose(f);
}

// ===== ADD MEMBER =====
void addMember(MemberManager *m) {
	if (m->size >= 1000) {
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
			m->members[m->size++] = mem;

			//Call save member to file function
			saveMemberToFile(m);

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

// ===== REMOVE MEMBER =====
/*
Input ID => Find by ID => If found, show member info 
=> Confirm to remove => If yes, remove by shift left array => Save to file
*/
void removeMember(MemberManager *m) {
	//Check if member list is empty
	if (m->size == 0) {
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
		for (int i = 0; i < m->size; i++) {
			if (strcmp(m->members[i].studentID, id) == 0) {
				
				//Save position to show student info before remove
				pos = i;
				break;
			}
		}

		if (pos != -1) {
			// Show student
			printf("\nStudent found:\n");
			printf("\n┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━┳━━━━━━┳━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━┓\n");
			printf("┃ %-11s ┃ %-20s ┃ %-25s ┃ %-12s ┃ %-5s ┃ %-5s ┃ %-15s ┃ %-20s ┃ %-11s ┃ %-10s ┃\n",
				"Student ID", "Name", "Email", "Phone", "Team", "Role", "Violation Count", "Consecutive Absences", "Total Fine", "Status");
			printf("┣━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━┫\n");

			printf("┃ %-11s ┃ %-20s ┃ %-25s ┃ %-12s ┃ %-5s ┃ %-5s ┃ %-15d ┃ %-20d ┃ %-11.2f ┃ %-10s ┃\n",
				m->members[pos].studentID,
				m->members[pos].fullName,
				m->members[pos].email,
				m->members[pos].phoneNumber,
				m->members[pos].team, 
				m->members[pos].role, 
				m->members[pos].violationCount,
				m->members[pos].consecutiveAbsences,
				m->members[pos].totalFine,
				m->members[pos].status);

			printf("┗━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━┻━━━━━━┻━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━┛\n");

			//Confirm to remove member
			int confirm;
			do {
				inputYesNo(&confirm,
						"\nRemove this member?\n1: Yes\n0: No\n=> Your choice: ");
			
			} while (!isOneOrZero(confirm));

			//Start to remove
			if (confirm == 1) {

				for (int i = pos; i < m->size - 1; i++){
					//Shift left array
					m->members[i] = m->members[i + 1];
				}
				//Decrease member size
				m->size--;

				//Call save member to file function
				saveMemberToFile(m);

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

// ===== UPDATE MEMBER =====
/*
Input ID => Find by ID => If found, show member info
=> ask which field want to update 
=> input new value => confirm to update 
=> If yes, update by assign new value to target member 
=> Save to file
*/
void updateMember(MemberManager *m){
	//Check if member list is empty
	if (m->size == 0) {
		printf("No members available to update.\n");
		return;
	}

	char fullName[50];
	char email[50];
	char phoneNumber[11];
	char studentID[9]; //SE000000
	char team [2]; // 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
	char role [2];
	int violationCount;
	int consecutiveAbsences;

	char id[9]; 
	int pos;

	int  continueUpdate = 1;
	while (continueUpdate) {
		pos = -1; //Reset position before find member
		inputStudentID(id, "Nhap studentID can cap nhat: ");

		//Find member by ID and update by assign new value to target member
		for (int i = 0; i < m->size; i++) {
			if (strcmp(m->members[i].studentID, id) == 0) {
				
				//Save position to show student info before update
				pos = i;
				break;
			}
		}

		if (pos != -1) {
			// Show student
			printf("\nStudent found:\n");
			printf("\n┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━┳━━━━━━┳━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━┓\n");
			printf("┃ %-11s ┃ %-20s ┃ %-25s ┃ %-12s ┃ %-5s ┃ %-5s ┃ %-15s ┃ %-20s ┃ %-11s ┃ %-10s ┃\n",
				"Student ID", "Name", "Email", "Phone", "Team", "Role", "Violation Count", "Consecutive Absences", "Total Fine", "Status");
			printf("┣━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━┫\n");

			printf("┃ %-11s ┃ %-20s ┃ %-25s ┃ %-12s ┃ %-5s ┃ %-5s ┃ %-15d ┃ %-20d ┃ %-11.2f ┃ %-10s ┃\n",
				m->members[pos].studentID,
				m->members[pos].fullName,
				m->members[pos].email,
				m->members[pos].phoneNumber,
				m->members[pos].team, 
				m->members[pos].role,
				m->members[pos].violationCount,
				m->members[pos].consecutiveAbsences,
				m->members[pos].totalFine,
				m->members[pos].status);
				printf("┗━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━┻━━━━━━┻━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━┛\n");

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
	 				inputMemberTeam(team, "Enter New Team (0-3): ");
					break;
					
				case 5:
					//Input new role info
					inputMemberRole(role, "Enter New Role (0-2): ");
					break;	
			}
		}
	}			
}

