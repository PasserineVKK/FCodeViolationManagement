#include "violation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/report.h"
#include "../include/utils.h"
#include "../include/view/viewUtil.h"
#include "../include/view/violationView.h"
#include "../include/violation.h"

#define IS_READ 1
#define DID_NOT_READ 0

void simpleDisplayViolation(Violation* v) {
    clearScreen();
    displayViolationList(v, 1);
}

int loadViolations(Violation violations[], int* count) {
    return loadFromFile(VIOLATIONS_FILE, violations, sizeof(Violation),
                        MAX_VIOLATIONS, count);
}

int saveViolations(Violation violations[], int count) {
    return saveToFile(VIOLATIONS_FILE, violations, sizeof(Violation), count);
}

double calculateFine(int role, int reason) {
    if (reason == REASON_VIOLENCE) return 0;
    if (role == 0) return 20000;
    return 50000;
}

int checkOutCondition(const Violation violations[], int count, const char* id) {
    int meetingAbsenceCount = 0;
    int violenceCount = 0;

    if (violations == NULL || count <= 0 || id == NULL || id[0] == '\0')
        return 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(violations[i].studentID, id) != 0) continue;

        if (violations[i].reason == REASON_MEETING_ABSENCE)
            meetingAbsenceCount++;
        else if (violations[i].reason == REASON_VIOLENCE)
            violenceCount++;

        if (meetingAbsenceCount >= 3 || violenceCount >= 1) return 1;
    }
    return 0;
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
    printf("Choose violate reason: \n");
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
    printf("%d %d\n", *count, *capacity);
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

    if (newV.reason == REASON_MEETING_ABSENCE) {
        m->consecutiveAbsences++;

        sprintf(content,
                "Member with ID %s. You have been absent from meetings. "
                "If this violation occurs more than twice, you will be "
                "kicked out. "
                "Please contact the administrator if the attendance record "
                "is incorrect.",
                studentID);
        warningMember(content, studentID, WILL_SAVE);

        if (m->consecutiveAbsences >= 3) {
            newV.penalty = PENALTY_KICK;
            m->isPending = 1;

            sprintf(content,
                    "Member with ID %s has missed more than three meetings. "
                    "Account automatically disabled; member will be "
                    "automatically deleted "
                    "unless there is further action.",
                    studentID);
            notifyAdmin(content, NULL, WILL_SAVE);

            sprintf(content,
                    "Member with ID %s. You have missed more than three "
                    "meetings, "
                    "so your account has been deactivated. You will be "
                    "kicked out of the club "
                    "in the future if you do not provide any satisfactory "
                    "explanation.",
                    studentID);
            warningMember(content, studentID, WILL_SAVE);
        }
    } else if (newV.reason == REASON_VIOLENCE) {
        m->isPending = 1;

        sprintf(content,
                "Member with ID %s. You have been kicked out for a "
                "violence-related violation.",
                studentID);
        warningMember(content, studentID, 1);
    }
}

void recordViolationView(Violation violations[], int* vCount, int* vCapacity,
                         Member members[], int mCount) {
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
    newV.isPending = NOT_PENDING;
    newV.penalty =
        (newV.reason == REASON_VIOLENCE) ? PENALTY_KICK : PENALTY_FINANCIAL;

    inputString(newV.note, 50, "Enter note (optional): ");

    int confirm;
    if (newV.reason == REASON_MEETING_ABSENCE ||
        newV.reason == REASON_VIOLENCE) {
        notifyAdmin(
            "You have just set a reason for violation that could lead to a "
            "member's removal. Please review it.",
            NULL, NOT_SAVE);
        printf("Chen giữa xem!\n");
    }
    inputYesNo(&confirm, "Confirm to record this violation? (1: Yes, 0: No): ");
    if (confirm) {
        if (addViolation(violations, vCount, newV)) {
            printf("Violation recorded successfully.\n");
            updateMemberTotalFine(members, mCount, violations, *vCount,
                                  studentID);
            saveViolations(violations, *vCount);
            saveMembers(members, mCount);
        } else {
            printf("Error: Violation list is full.\n");
        }
    }

    handleSeriousViolation(&members[mIndex], newV);
}

// 2.8 Check and warn if member reach out club condition
//  int checkAndWarnOutClub(const char *studentID, Member members [], int
//  *mCount, Violation violations[], int *vCount) {
//      int mIndex = searchMemberByIdInM(members, *mCount, studentID);
//      if (mIndex == -1) return 0;

//     int absences = members[mIndex].consecutiveAbsences;
//     int hasViolence = 0;

//     for (int i = *vCount - 1; i >= 0; i--) {
//         if (strcmp(violations[i].studentID, studentID) == 0 &&
//         violations[i].reason == REASON_VIOLENCE) {
//             hasViolence = 1;
//             break;
//         }
//     }

//     //case: violence -> direct out
//     if(hasViolence){
//         printf("\n!!! Warning: Member %s has committed violence and is
//         subject to removal from the club. Please review the violation
//         details and take necessary actions.\n", studentID); int confirm;

//         inputYesNo(&confirm, "Confirm to remove this member from the
//         club?\n1: Yes\n0: No\n=> Your choice: "); if(confirm == 1){
//             members[mIndex].isPending = 1; //set pending for out club
//             for (int i = 0; i < *vCount; i++) {
//                 if (strcmp(violations[i].studentID, studentID) == 0
//                     && violations[i].reason == REASON_VIOLENCE
//                     && violations[i].pelnaty == 0) {
//                     violations[i].pelnaty = 1; //set kick for violence
//                     violation violations[i].fine = 0;
//                 }
//             }
//             saveMembers(members, *mCount);
//             saveViolations(violations, *vCount);
//             printf("Added %s to pending out club list.\n", studentID);
//             return 2;
//         }
//         return 1;
//     }

//     //case: Consecutive absences >= 2 -> warning
//     //      -> if reach 3 -> bcn confirm out
//     if(absences >= 2){
//         printf("\n!!! Warning: Member %s has %d consecutive absences and
//         is approaching the threshold for removal from the club. Please
//         review the attendance records and take necessary actions.\n",
//         studentID, absences); if (absences >= 3){
//             int confirm;
//             inputYesNo(&confirm, "Confirm to remove this member from the
//             club?\n1: Yes\n0: No\n=> Your choice: "); if(confirm == 1){
//                 members[mIndex].isPending = 1;
//                 saveMembers(members, *mCount);
//                 printf("Added %s to pending out club list.\n",
//                 studentID); return 2;
//             }
//         }
//         return 1;
//     }
//     return 0;
// }