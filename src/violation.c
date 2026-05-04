
#include "../include/violation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/model.h"
#include "../include/report.h"
#include "../include/utils.h"
#include "../include/view/viewUtil.h"
#include "../include/view/violationView.h"
#include "../include/view/memberView.h"
#include "../include/validate.h"


#define IS_READ 1
#define DID_NOT_READ 0

void simpleDisplayViolation(Violation* v) {
	clearScreen();
	displayViolationList(v, 1);
}


void initViolationList(ViolationList *list, int initialCapacity){
	list->count = 0;
	list->capacity = initialCapacity;
	list->data = malloc(sizeof(Violation) * list->capacity);
}

int loadViolations(ViolationList *violations, MemberList *members) {
	int isLoadSuccess = loadFromFile(VIOLATIONS_FILE, violations->data, sizeof(Violation) - sizeof(Member*),
	                    MAX_VIOLATIONS, &violations->count);
	                    
	if (!isLoadSuccess){
		printf("No initial data");
		violations->count = 0;
		return 1;
	} else {
	// assign pointer to owner
		int mCount = members->count;
		for (int i = 0; i < violations->count; i++){
			int mIndex = searchMemberByIdInM(members->data, mCount, violations->data[i].studentID);
			//check wheter mIndex valid
			if (mIndex>=0 && mIndex <mCount){
				violations->data[i].owner = &(members->data[mIndex]); //owner is pointer
			} else {
				violations->data[i].owner = NULL;	
			}
		}
	}
	return 1;
}

int saveViolations(Violation violations[], int count) {
	for(int i = 0; i < count; i++) {
        violations[i].owner = NULL;
    }
	return saveToFile(VIOLATIONS_FILE, violations, sizeof(Violation) - sizeof(Member*), count);
}

double calculateFine(int role, int reason) {
	if (reason == REASON_VIOLENCE) return 0;
	if (role == 0) return 20000;
	return 50000;
}

void refreshFineAfterRolechange(char* memberId, int role,
                                Violation violations[], int count) {
	for (int i = 0; i < count; i++) {
		if (strcmp(violations[i].studentID, memberId) == 0 &&
		        violations[i].isPaid == NOT_PAY)
			violations[i].fine = calculateFine(role, violations[i].reason);
	}
}

Violation* findViolationById(const char* violationId, Violation violations[],
                             int count) {
	for (int i = 0; i < count; i++)
		if (strcmp(violations[i].violationID, violationId) == 0)
			return &violations[i];
	return NULL;
}

int getViolationIndexById(Violation violations[], int count,
                          const char* violationId) {
	for (int i = 0; i < count; i++)
		if (strcmp(violations[i].violationID, violationId) == 0) return i;
	return -1;
}

int searchMemberByIdInV(Violation violations[], int count, const char* id) {
	for (int i = 0; i < count; i++)
		if (strcmp(violations[i].studentID, id) == 0) return i;
	return -1;
}

void getReason(int* reason) {
	printf("Choose violate reason: \n", &reason);
	printf("%d - Not uniform\n", REASON_NOT_UNIFORM);
	printf("%d - Meeting absence\n", REASON_MEETING_ABSENCE);
	printf("%d - Not join in Club activity\n", REASON_NO_CLUB_ACTIVITY);
	printf("%d - Violence\n", REASON_VIOLENCE);
	inputIntegerInRange(reason, 0, 3, "Please enter: ");
}

int addViolation(Violation violations[], int* count, Violation newV) {
	simpleDisplayViolation(&newV);
	if (*count >= MAX_VIOLATIONS) return 0;
	violations[*count] = newV;
	(*count)++;
	return 1;
}

void updateIsPaidField(char* violationId, Violation violations[], int count,
                       int value) {
	Violation* v = findViolationById(violationId, violations, count);
	if (v != NULL) v->isPaid = value;
}

void ensureCapacity(Violation** violations, int* count, int* capacity) {
	if (*count >= *capacity) {
		*capacity = *capacity * 2;
		Violation* anotherAddress =
		    realloc(*violations, *capacity * sizeof(Violation));

		if (anotherAddress == NULL) {
			fprintf(stderr,
			        "NOT ENOUGH MEMORY TO ALLOCATE (REDIRECT FROM CREATE NEW "
			        "VIOLATION LIST)!!!");
			return;
		}

		*violations = anotherAddress;
	}
}

void createViolationID(int index, char* buffer) {
	sprintf(buffer, "VI%04d", index);
}

void deleteViolation(Violation* violations, int* count, Violation* v) {
	if (violations == NULL || v == NULL || *count <= 0) return;
	for (int i = 0; i < *count; i++) {
		if (&violations[i] == v) {
			for (int j = i; j < *count - 1; j++) {
				violations[j] = violations[j + 1];
			}
			(*count)--;
			break;
		}
	}
}

void handleSeriousViolation(Member* m, Violation newV) {
	char content[200];
	char* studentID = m->studentID;

	if(m == NULL) return;
	if (newV.reason == REASON_MEETING_ABSENCE) {
		sprintf(content,
		        "Member with ID %s. You have been absent from meetings. "
		        "If this violation occurs more than twice, you will be "
		        "kicked out. "
		        "Please contact the administrator if the attendance record "
		        "is incorrect.",
		        studentID);
		warningMember(content, studentID, WILL_SAVE);
	} else if (newV.reason == REASON_VIOLENCE) {
		sprintf(content,
		        "Member with ID %s. You have been kicked out for a "
		        "violence-related violation.",
		        studentID);
		warningMember(content, studentID, 1);
	}
}

void recordViolationView(Violation violations[], int* vCount, int* vCapacity,
                         Member members[], int mCount) {
	int continueRecord = 1;
	while (continueRecord) {
		ensureCapacity(&violations, vCount, vCapacity);

		Violation newV;
		char studentID[10];
		int mIndex;

		printf("\n--- Record New Violation ---\n");
		inputStudentID(studentID, "Enter Student ID: ");

		mIndex = searchMemberByIdInM(members, mCount, studentID);
		if (mIndex == -1) {
			printf("Error: Student ID not found.\n");
			return;
		}

		strcpy(newV.studentID, studentID);
		sprintf(newV.violationID, "VIO%03d", *vCount + 1);

		printf("Reasons:\n");
		printf("%d. Not uniform\n", REASON_NOT_UNIFORM);
		printf("%d. Meeting absence\n", REASON_MEETING_ABSENCE);
		printf("%d. No Club activity\n", REASON_NO_CLUB_ACTIVITY);
		printf("%d. Violence\n", REASON_VIOLENCE);
		inputIntegerInRange(&newV.reason, 0, 3, "Enter reason: ");

		newV.violationTime = time(NULL);
		newV.fine = calculateFine(members[mIndex].role, newV.reason);
		newV.isPaid = NOT_PAY;
		newV.penalty = PENALTY_FINANCIAL;
		newV.isPending = NOT_PENDING;

		inputString(newV.note, 100, "Enter note (optional): ");

		if (newV.reason == REASON_MEETING_ABSENCE) {
			members[mIndex].consecutiveAbsences++;
		}

		int confirm;
		inputYesNo(&confirm, "Confirm to record this violation? (1: Yes, 0: No): ");
		if (!confirm) {
			// rollback
			if (newV.reason == REASON_MEETING_ABSENCE) {
				members[mIndex].consecutiveAbsences--;
			}
			printf("Violation not recorded.\n");
		} else {
			if (!addViolation(violations, vCount, newV)) {
				printf("Error: Violation list is full.\n");
				return;
			}

			if (newV.reason == REASON_MEETING_ABSENCE || newV.reason == REASON_VIOLENCE) {
				handleSeriousViolation(&members[mIndex], newV);
			}

			updateMemberTotalFine(members, mCount, violations, *vCount, studentID);
			//se
			if (members[mIndex].consecutiveAbsences >= 3 ||
			        hasViolenceViolation(studentID, violations, *vCount)) {
				members[mIndex].isPending = 1;
			}
			saveViolations(violations, *vCount);
			saveMembers(members, mCount);

			printf("Violation recorded successfully.\n");
		}

		inputYesNo(&continueRecord, "\nRecord another violation?\n1: Yes\n0: No\n=> Your choice: ");
	}
}

// 2.8 Check and warn if member reach out club condition
void checkAndWarnOutClub(Member members[], int* mCount,
                         Account accounts[], int* aCount,
                         Violation violations[], int* vCount) {
	int continueProcess = 1;

	while (continueProcess) {
		displayWarningList(members, *mCount, violations, *vCount);
		displayKickList(members, *mCount, violations, *vCount);

		char studentID[10];
		inputString(studentID, 10, "\nEnter student ID from kick list to process (or press Enter to back): ");

		if (strcmp(studentID, "") == 0) {
			return;
		}

		//validate studentID
		if (!isValidStudentID(studentID)) continue;

		int mIndex = searchMemberByIdInM(members, *mCount, studentID);
		if (mIndex == -1) {
			printf("Member not found.\n");
			continue;
		}

		if (members[mIndex].isPending != 1) {
			printf("This member is not in kick list.\n");
			continue;
		}

		printf("\nSelected Member:\n");
		displayOneMemberInfo(members[mIndex]);

		int confirm;
		inputYesNo(&confirm,
		           "\nConfirm kick this member and remove all related data?\n1. Yes\n0. No\n=> Your choice: ");

		if (confirm == 1) {
			char targetID[10];
			strcpy(targetID, members[mIndex].studentID);

			removeMemberById(targetID,
			                 members, mCount,
			                 accounts, aCount,
			                 violations, vCount);

			printf("Member kicked and removed successfully.\n");
		} else {
			printf("No action taken.\n");
		}

		inputYesNo(&continueProcess, "\nDo you want to process another member?\n1. Yes\n0. No\n=> Your choice: ");
	}
}

// Display members in warning list
void displayWarningList(Member members[], int mCount, Violation violations[], int vCount) {
	printf("\n===== WARNING LIST =====\n");
	printf("┌─────────────┬──────────────────────┬────────────────────┐\n");
	printf("│ %-11s │ %-20s │ %-18s │\n", "Student ID", "Full Name", "Consecutive Absence");
	printf("├─────────────┼──────────────────────┼────────────────────┤\n");

	int found = 0;
	for (int i = 0; i < mCount; i++) {
		if (isMemberInWarningList(members[i], violations, vCount)) {
			printf("│ %-11s │ %-20s │ %-18d │\n",
			       members[i].studentID,
			       members[i].fullName,
			       members[i].consecutiveAbsences);
			found = 1;
		}
	}
	if (!found) {
		printf("│ %-52s │\n", "No members in warning list.");
	}
	printf("└─────────────┴──────────────────────┴────────────────────┘\n");
}

// Display members in kick list
void displayKickList(Member members[], int mCount, Violation violations[], int vCount) {
	printf("\n===== KICK LIST =====\n");
	printf("┌─────────────┬──────────────────────┬─────────────────────┬──────────────────────┐\n");
	printf("│ %-11s │ %-20s │ %-19s │ %-20s │\n",
	       "Student ID", "Full Name", "Consecutive Absence", "Reason");
	printf("├─────────────┼──────────────────────┼─────────────────────┼──────────────────────┤\n");

	int found = 0;
	for (int i = 0; i < mCount; i++) {
		if (members[i].isPending == 1) {
			int hasViolence  = hasViolenceViolation(members[i].studentID, violations, vCount);
			int hasAbsence   = members[i].consecutiveAbsences >= 3;

			// Xây reason string dựa trên cả 2 điều kiện
			char reason[40] = "";
			if (hasAbsence && hasViolence) {
				strcpy(reason, "Absence + Violence");
			} else if (hasViolence) {
				strcpy(reason, "Violence");
			} else {
				strcpy(reason, "Absence Threshold");
			}

			printf("│ %-11s │ %-20s │ %-19d │ %-20s │\n",
			       members[i].studentID,
			       members[i].fullName,
			       members[i].consecutiveAbsences,
			       reason);
			found = 1;
		}
	}

	if (!found) {
		printf("│ %-73s │\n", "No members in kick list.");
	}
	printf("└─────────────┴──────────────────────┴─────────────────────┴──────────────────────┘\n");
}

// Check if member has any violence-related violation
int hasViolenceViolation(const char* studentID, Violation violations[], int vCount) {
	for (int i = 0; i < vCount; i++) {
		if (strcmp(violations[i].studentID, studentID) == 0 &&
		        violations[i].reason == REASON_VIOLENCE) {
			return 1;
		}
	}
	return 0;
}

int isMemberInWarningList(Member member, Violation violations[], int vCount) {
	if (member.isPending == 1) return 0;
	if (member.consecutiveAbsences == 2) return 1;
	return 0;
}

//remove member in pending list after confirm kick by BCN
void removeMemberById(const char* id, Member members[], int* mCount, Account accounts[], int* aCount, Violation violations[], int* vCount) {
	int mIndex = searchMemberByIdInM(members, *mCount, id);
	int aIndex = searchMemberByIdInA(accounts, *aCount, id);

	if (mIndex == -1) {
		printf("Member not found.\n");
		return;
	}

	for (int i = mIndex; i < *mCount - 1; i++) {
		members[i] = members[i + 1];
	}
	(*mCount)--;

	if (aIndex != -1) {
		for (int i = aIndex; i < *aCount - 1; i++) {
			accounts[i] = accounts[i + 1];
		}
		(*aCount)--;
	}

	for (int i = 0; i < *vCount; i++) {
		if (strcmp(violations[i].studentID, id) == 0) {
			for (int j = i; j < *vCount - 1; j++) {
				violations[j] = violations[j + 1];
			}
			(*vCount)--;
			i--;
		}
	}

	deleteNotificationByMemberId(id);

	saveMembers(members, *mCount);
	saveAccounts(accounts, *aCount);
	saveViolations(violations, *vCount);
}
