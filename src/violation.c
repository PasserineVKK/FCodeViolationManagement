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

void simpleDisplayViolation(const Violation* v) {
    clearScreen();
    // Assuming displayViolationList handles a single item if count is 1
    displayViolationList((Violation*)v, 1); 
}

void initViolationList(ViolationList *list, int initialCapacity){
    list->count = 0;
    list->capacity = initialCapacity;
    list->data = malloc(sizeof(Violation) * list->capacity);
}

int loadViolations(ViolationList *violations, const MemberList *members) {
    int isLoadSuccess = loadFromFile(VIOLATIONS_FILE, violations->data, sizeof(Violation) - sizeof(Member*),
                                    MAX_VIOLATIONS, &violations->count);
                                    
    if (!isLoadSuccess){
        printf("No initial data");
        violations->count = 0;
        return 1;
    } else {
        // assign pointer to owner
        for (int i = 0; i < violations->count; i++){
            int mIndex = searchMemberByIdInM(members->data, members->count, violations->data[i].studentID);
            // check whether mIndex valid
            if (mIndex >= 0 && mIndex < members->count){
                // owner is pointer
                violations->data[i].owner = (Member*)&(members->data[mIndex]); 
            } else {
                violations->data[i].owner = NULL;   
            }
        }
    }
    return 1;
}

int saveViolations(ViolationList* violations) {
    // Note: Setting owner = NULL modifies the runtime data to prevent invalid addresses 
    // being saved, but it also breaks the links in memory after saving. 
    // Kept as original logic strictly.
    for(int i = 0; i < violations->count; i++) {
        violations->data[i].owner = NULL;
    }
    return saveToFile(VIOLATIONS_FILE, violations->data, sizeof(Violation) - sizeof(Member*), violations->count);
}

double calculateFine(int role, int reason) {
    if (reason == REASON_VIOLENCE) return 0;
    if (role == 0) return 20000;
    return 50000;
}

void refreshFineAfterRolechange(const char* memberId, int role, ViolationList* violations) {
    for (int i = 0; i < violations->count; i++) {
        Violation* v = &violations->data[i];
        if (strcmp(v->studentID, memberId) == 0 && v->isPaid == NOT_PAY) {
            v->fine = calculateFine(role, v->reason);
        }
    }
}

Violation* findViolationById(const char* violationId, ViolationList* violations) {
    for (int i = 0; i < violations->count; i++) {
        if (strcmp(violations->data[i].violationID, violationId) == 0) {
            return &violations->data[i];
        }
    }
    return NULL;
}

int getViolationIndexById(const ViolationList* violations, const char* violationId) {
    for (int i = 0; i < violations->count; i++) {
        if (strcmp(violations->data[i].violationID, violationId) == 0) return i;
    }
    return -1;
}

int searchMemberByIdInV(const ViolationList* violations, const char* id) {
    for (int i = 0; i < violations->count; i++) {
        if (strcmp(violations->data[i].studentID, id) == 0) return i;
    }
    return -1;
}

void getReason(int* reason) {
    printf("Choose violate reason: \n"); // Fixed redundant format arg
    printf("%d - Not uniform\n", REASON_NOT_UNIFORM);
    printf("%d - Meeting absence\n", REASON_MEETING_ABSENCE);
    printf("%d - Not join in Club activity\n", REASON_NO_CLUB_ACTIVITY);
    printf("%d - Violence\n", REASON_VIOLENCE);
    inputIntegerInRange(reason, 0, 3, "Please enter: ");
}

int addViolation(ViolationList* violations, const Violation* newV) {
    simpleDisplayViolation(newV);
    // Check capacity against list->capacity instead of static MAX_VIOLATIONS
    if (violations->count >= violations->capacity) return 0; 
    
    // Use pointer copy directly into memory block
    violations->data[violations->count] = *newV;
    violations->count++;
    return 1;
}

void updateIsPaidField(const char* violationId, ViolationList* violations, int value) {
    Violation* v = findViolationById(violationId, violations);
    if (v != NULL) v->isPaid = value;
}

void ensureCapacity(ViolationList* list) {
    if (list->count >= list->capacity) {
        list->capacity = list->capacity * 2;
        Violation* anotherAddress =
            realloc(list->data, list->capacity * sizeof(Violation));

        if (anotherAddress == NULL) {
            fprintf(stderr,
                    "NOT ENOUGH MEMORY TO ALLOCATE (REDIRECT FROM CREATE NEW "
                    "VIOLATION LIST)!!!");
            return;
        }

        list->data = anotherAddress;
    }
}

void createViolationID(int index, char* buffer) {
    sprintf(buffer, "VI%04d", index);
}

void deleteViolation(ViolationList* list, const Violation* v) {
    if (list == NULL || list->data == NULL || v == NULL || list->count <= 0) return;
    for (int i = 0; i < list->count; i++) {
        if (&list->data[i] == v) {
            for (int j = i; j < list->count - 1; j++) {
                list->data[j] = list->data[j + 1];
            }
            list->count--;
            break;
        }
    }
}

void handleSeriousViolation(const Member* m, const Violation* newV) {
    char content[200];
    const char* studentID = m->studentID;

    if(m == NULL) return;
    if (newV->reason == REASON_MEETING_ABSENCE) {
        sprintf(content,
                "Member with ID %s. You have been absent from meetings. "
                "If this violation occurs more than twice, you will be "
                "kicked out. "
                "Please contact the administrator if the attendance record "
                "is incorrect.",
                studentID);
        warningMember(content, studentID, WILL_SAVE);
    } else if (newV->reason == REASON_VIOLENCE) {
        sprintf(content,
                "Member with ID %s. You have been kicked out for a "
                "violence-related violation.",
                studentID);
        warningMember(content, studentID, 1);
    }
}

void recordViolationView(ViolationList* violations, MemberList* members) {
    int continueRecord = 1;
    while (continueRecord) {
        ensureCapacity(violations);

        Violation newV;
        char studentID[10];
        int mIndex;

        printf("\n--- Record New Violation ---\n");
        inputStudentID(studentID, "Enter Student ID: ");

        mIndex = searchMemberByIdInM(members->data, members->count, studentID);
        if (mIndex == -1) {
            printf("Error: Student ID not found.\n");
            return;
        }

        // Pointer directly points to the member in array to avoid shallow copy
        Member* targetMem = &members->data[mIndex];

        strcpy(newV.studentID, studentID);
        sprintf(newV.violationID, "VIO%03d", violations->count + 1);

        printf("Reasons:\n");
        printf("%d. Not uniform\n", REASON_NOT_UNIFORM);
        printf("%d. Meeting absence\n", REASON_MEETING_ABSENCE);
        printf("%d. No Club activity\n", REASON_NO_CLUB_ACTIVITY);
        printf("%d. Violence\n", REASON_VIOLENCE);
        inputIntegerInRange(&newV.reason, 0, 3, "Enter reason: ");

        newV.violationTime = time(NULL);
        newV.fine = calculateFine(targetMem->role, newV.reason);
        newV.isPaid = NOT_PAY;
        newV.penalty = PENALTY_FINANCIAL;
        newV.isPending = NOT_PENDING;

        inputString(newV.note, 100, "Enter note (optional): ");

        if (newV.reason == REASON_MEETING_ABSENCE) {
            targetMem->consecutiveAbsences++;
        }

        int confirm;
        inputYesNo(&confirm, "Confirm to record this violation? (1: Yes, 0: No): ");
        if (!confirm) {
            // rollback
            if (newV.reason == REASON_MEETING_ABSENCE) {
                targetMem->consecutiveAbsences--;
            }
            printf("Violation not recorded.\n");
        } else {
            if (!addViolation(violations, &newV)) {
                printf("Error: Violation list is full.\n");
                return;
            }

            // Assign the owner pointer strictly to the newly added record
            violations->data[violations->count - 1].owner = targetMem;

            if (newV.reason == REASON_MEETING_ABSENCE || newV.reason == REASON_VIOLENCE) {
                handleSeriousViolation(targetMem, &newV);
            }

            updateMemberTotalFine(members, violations, studentID);
            
            if (targetMem->consecutiveAbsences >= 3 ||
                hasViolenceViolation(studentID, violations)) {
                targetMem->isPending = 1;
            }
            
            saveViolations(violations);
            saveMembers(members);

            printf("Violation recorded successfully.\n");
        }

        inputYesNo(&continueRecord, "\nRecord another violation?\n1: Yes\n0: No\n=> Your choice: ");
    }
}

// 2.8 Check and warn if member reach out club condition
void checkAndWarnOutClub(MemberList* members, AccountList* accounts, ViolationList* violations) {
    int continueProcess = 1;

    while (continueProcess) {
        displayWarningList(members, violations);
        displayKickList(members, violations);

        char studentID[10];
        inputString(studentID, 10, "\nEnter student ID from kick list to process (or press Enter to back): ");

        if (strcmp(studentID, "") == 0) {
            return;
        }

        //validate studentID
        if (!isValidStudentID(studentID)) continue;

        int mIndex = searchMemberByIdInM(members->data, members->count, studentID);
        if (mIndex == -1) {
            printf("Member not found.\n");
            continue;
        }

        Member* targetMem = &members->data[mIndex];

        if (targetMem->isPending != 1) {
            printf("This member is not in kick list.\n");
            continue;
        }

        printf("\nSelected Member:\n");
        displayOneMemberInfo(*targetMem);

        int confirm;
        inputYesNo(&confirm,
                   "\nConfirm kick this member and remove all related data?\n1. Yes\n0. No\n=> Your choice: ");

        if (confirm == 1) {
            char targetID[10];
            strcpy(targetID, targetMem->studentID);

            removeMemberById(targetID, members, accounts, violations);

            printf("Member kicked and removed successfully.\n");
        } else {
            printf("No action taken.\n");
        }

        inputYesNo(&continueProcess, "\nDo you want to process another member?\n1. Yes\n0. No\n=> Your choice: ");
    }
}

// Display members in warning list
void displayWarningList(const MemberList* members, const ViolationList* violations) {
    printf("\n===== WARNING LIST =====\n");
    printf("+-------------------------------------------------------+\n");
    printf("¦ %-11s ¦ %-20s ¦ %-18s ¦\n", "Student ID", "Full Name", "Consecutive Absence");
    printf("+-----------+----------------------+--------------------¦\n");

    int found = 0;
    for (int i = 0; i < members->count; i++) {
        if (isMemberInWarningList(&members->data[i], violations)) {
            printf("¦ %-11s ¦ %-20s ¦ %-18d ¦\n",
                   members->data[i].studentID,
                   members->data[i].fullName,
                   members->data[i].consecutiveAbsences);
            found = 1;
        }
    }
    if (!found) {
        printf("¦ %-52s ¦\n", "No members in warning list.");
    }
    printf("+-------------------------------------------------------+\n");
}

// Display members in kick list
void displayKickList(const MemberList* members, const ViolationList* violations) {
    printf("\n===== KICK LIST =====\n");
    printf("+-------------------------------------------------------------------------------+\n");
    printf("¦ %-11s ¦ %-20s ¦ %-19s ¦ %-20s ¦\n",
           "Student ID", "Full Name", "Consecutive Absence", "Reason");
    printf("+-----------+----------------------+---------------------+----------------------¦\n");

    int found = 0;
    for (int i = 0; i < members->count; i++) {
        const Member* mem = &members->data[i];
        if (mem->isPending == 1) {
            int hasViolence  = hasViolenceViolation(mem->studentID, violations);
            int hasAbsence   = mem->consecutiveAbsences >= 3;

            // Xây reason string d?a trên c? 2 di?u ki?n
            char reason[40] = "";
            if (hasAbsence && hasViolence) {
                strcpy(reason, "Absence + Violence");
            } else if (hasViolence) {
                strcpy(reason, "Violence");
            } else {
                strcpy(reason, "Absence Threshold");
            }

            printf("¦ %-11s ¦ %-20s ¦ %-19d ¦ %-20s ¦\n",
                   mem->studentID,
                   mem->fullName,
                   mem->consecutiveAbsences,
                   reason);
            found = 1;
        }
    }

    if (!found) {
        printf("¦ %-73s ¦\n", "No members in kick list.");
    }
    printf("+-------------------------------------------------------------------------------+\n");
}

// Check if member has any violence-related violation
int hasViolenceViolation(const char* studentID, const ViolationList* violations) {
    for (int i = 0; i < violations->count; i++) {
        if (strcmp(violations->data[i].studentID, studentID) == 0 &&
            violations->data[i].reason == REASON_VIOLENCE) {
            return 1;
        }
    }
    return 0;
}

int isMemberInWarningList(const Member* member, const ViolationList* violations) {
    if (member->isPending == 1) return 0;
    if (member->consecutiveAbsences == 2) return 1;
    return 0;
}

//remove member in pending list after confirm kick by BCN
void removeMemberById(const char* id, MemberList* members, AccountList* accounts, ViolationList* violations) {
    int mIndex = searchMemberByIdInM(members->data, members->count, id);
    int aIndex = searchMemberByIdInA(accounts, id);

    if (mIndex == -1) {
        printf("Member not found.\n");
        return;
    }

    for (int i = mIndex; i < members->count - 1; i++) {
        members->data[i] = members->data[i + 1];
    }
    members->count--;

    if (aIndex != -1) {
        for (int i = aIndex; i < accounts->count - 1; i++) {
            accounts->data[i] = accounts->data[i + 1];
        }
        accounts->count--;
    }

    for (int i = 0; i < violations->count; i++) {
        if (strcmp(violations->data[i].studentID, id) == 0) {
            for (int j = i; j < violations->count - 1; j++) {
                violations->data[j] = violations->data[j + 1];
            }
            violations->count--;
            i--;
        }
    }

    deleteNotificationByMemberId(id);

    saveMembers(members);
    saveAccounts(accounts); // Ensure signature matches in auth/account module
    saveViolations(violations);
}
