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
		printf("Danh sach day!\n");
		return;
	}

	char fullName[50];
	char email[50];
	char phoneNumber[11];
	char studentID[9]; //SE000000
	char team [2]; // 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
	char role [2];
    
	int continueAdd = 1;

	while (continueAdd) {
		do {
			printf("Enter Name: ");
			// Scan all name
			fgets(fullName, sizeof(fullName), stdin);

			// Check '\n', if don't have, string longer than buffer can get => fail
			if (!strchr(fullName, '\n'))
				//Read all until '\n' to make buffer clean for next input
				while (getchar() != '\n');

			// Replace '\n' by '\0'
			fullName[strcspn(fullName, "\n")] = '\0';

		} while (!isValidName(fullName));

		do {
			printf("Enter studnet ID: ");
			// Scan all student ID
			fgets(studentID, sizeof(studentID), stdin);

			// Check '\n', if don't have, string longer than buffer can get => fail
			if (!strchr(studentID, '\n'))
				//Read all until '\n' to make buffer clean for next input
				while (getchar() != '\n');

			// Replace '\n' by '\0'
			studentID[strcspn(studentID, "\n")] = '\0';

		} while (!isValidMemberID(studentID));

		do {
			printf("Enter email: ");
			// Scan all email
			fgets(email, sizeof(email), stdin);

			// Check '\n', if don't have, string longer than buffer can get => fail
			if (!strchr(email, '\n'))
				//Read all until '\n' to make buffer clean for next input
				while (getchar() != '\n');

			// Replace '\n' by '\0'
			email[strcspn(email, "\n")] = '\0';

		} while (!isValidEmail(email));

		do {
			printf("Enter Phone Number: ");
			// Scan all phone number
			fgets(phoneNumber, sizeof(phoneNumber), stdin);

			// Check '\n', if don't have, string longer than buffer can get => fail
			if (!strchr(phoneNumber, '\n'))
				//Read all until '\n' to make buffer clean for next input
				while (getchar() != '\n');

			// Replace '\n' by '\0'
			phoneNumber[strcspn(phoneNumber, "\n")] = '\0';

		} while (!isValidPhone(phoneNumber));


		do {
			printf("Enter Team (0-3): ");
			// Scan all team string
			fgets(team, sizeof(team), stdin);

			// Check '\n', if don't have, string longer than buffer can get => fail
			if (!strchr(team, '\n'))
				//Read all until '\n' to make buffer clean for next input
				while (getchar() != '\n');

			// Replace '\n' by '\0'
			team[strcspn(team, "\n")] = '\0';
		} while (!isValidTeam(team));

		do {
			printf("Enter Role (0-2): ");
			// Scan all role string
			fgets(role, sizeof(role), stdin);

			// Check '\n', if don't have, string longer than buffer can get => fail
			if (!strchr(role, '\n'))
				//Read all until '\n' to make buffer clean for next input
				while (getchar() != '\n');

			// Replace '\n' by '\0'
			role[strcspn(role, "\n")] = '\0';

		} while (!isValidRole(role));

		int confirm;
		do {
			inputInteger(&confirm,
			         "\nAdd this member?\n1: Yes\n0: No\n?? Your choice: ");

			if (confirm != 0 && confirm != 1) {
				printf("? Only 0 or 1!\n");
			}
		} while (confirm != 0 && confirm != 1);

		if (confirm == 1) {

			Member mem;

			strcpy(mem.fullName, fullName);
			strcpy(mem.studentID, studentID);
			strcpy(mem.email, email);
			strcpy(mem.phoneNumber, phoneNumber);

			mem.team = atoi(team);
			mem.role = atoi(role);

			mem.violationCount = 0;
			mem.consecutiveAbsences = 0;
			mem.totalFine = 0;
			mem.status = 1;

			// ADD + SAVE
			m->members[m->size++] = mem;

			saveMemberToFile(m);

			printf("? Added + Saved!\n");

		} else {
			printf("? Discarded.\n");
		}

		// ===== CONTINUE =====
		char choice[10];

		do {
			printf("\nAdd another member? (y/n): ");
			fgets(choice, sizeof(choice), stdin);

			if (!strchr(choice, '\n'))
				while (getchar() != '\n');

			choice[strcspn(choice, "\n")] = '\0';

		} while (strcmp(choice, "y") != 0 && strcmp(choice, "n") != 0);

		if (strcmp(choice, "n") == 0) {
			continueAdd = 0;
		}
	}
}



void removeMember(MemberManager *m) {
	char id[20];
	printf("Nhap studentID can xoa: ");
	scanf("%s", id);

	for (int i = 0; i < m->size; i++) {
		if (strcmp(m->members[i].studentID, id) == 0) {

			// d?n m?ng (shift left)
			for (int j = i; j < m->size - 1; j++) {
				m->members[j] = m->members[j + 1];
			}

			m->size--;

			printf("Xoa thanh cong!\n");
			return;
		}
	}

	printf("Khong tim thay member!\n");
}
