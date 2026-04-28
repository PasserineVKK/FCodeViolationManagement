#include "../../include/view/violationView.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../include/consoleInput.h"
#include "../../include/report.h"
#include "../../include/view/viewUtil.h"

void displayViolationTableHeader() {
    printf(
        "\n┏━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┓\n");
    printf(
        "┃ %-10s ┃ %-10s ┃ %-20s ┃ %-20s ┃ %-8s ┃ %-8s ┃ %-10s ┃ %-32s ┃ %-12s ┃\n",
        "ID", "Student", "Reason", "Time", "Fine", "Paid", "Penalty", "Note", "Pending");
    printf(
        "┣━━━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━┫\n");
}

void displayViolationTableFooter() {
    printf(
        "┗━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┛\n");
}

void displayViolationRow(Violation* v) {
    char timeField[30];
    getFormatTime(timeField, 30, v->violationTime);

    const char* reasonStr;
    switch (v->reason) {
        case REASON_NOT_UNIFORM:
            reasonStr = "Not uniform";
            break;
        case REASON_MEETING_ABSENCE:
            reasonStr = "Meeting absence";
            break;
        case REASON_NO_CLUB_ACTIVITY:
            reasonStr = "No Club activity";
            break;
        case REASON_VIOLENCE:
            reasonStr = "Violence";
            break;
        default:
            reasonStr = "Unknown";
            break;
    }

    const char* paidStr = v->isPaid ? "Yes" : "No";
    const char* penaltyStr = v->penalty ? "Kick" : "Financial";
    const char* pendingStr = v->isPending ? "Pending" : "Resolved";

    printf(
        "┃ %-10s ┃ %-10s ┃ %-20s ┃ %-20s ┃ %-8.0f ┃ %-8s ┃ %-10s ┃ %-32s ┃ "
        "%-12s ┃\n",
        v->violationID, v->studentID, reasonStr, timeField, v->fine, paidStr,
        penaltyStr, v->note, pendingStr);
}

void displayViolationList(Violation violations[], int vCount) {
    displayViolationTableHeader();
    for (int i = 0; i < vCount; i++) displayViolationRow(&violations[i]);
    displayViolationTableFooter();
}

void displayViolationByStudentId(char* id, Violation violations[], int vCount) {
    displayViolationTableHeader();
    for (int i = 0; i < vCount; i++) {
        if (strcmp(id, violations[i].studentID) == 0)
            displayViolationRow(&violations[i]);
    }
    displayViolationTableFooter();
}

void recordViolationView(Violation violations[], int* vCount, Member members[],
                         int mCount) {
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

    if (newV.reason == REASON_MEETING_ABSENCE) {
        members[mIndex].consecutiveAbsences++;
        if (members[mIndex].consecutiveAbsences >= 3) {
            newV.penalty = PENALTY_KICK;
            members[mIndex].isPending = 1;
        }
    } else {
        members[mIndex].consecutiveAbsences = 0;
    }

    printf("Enter note (optional): ");
    while (getchar() != '\n');
    fgets(newV.note, 100, stdin);
    newV.note[strcspn(newV.note, "\n")] = 0;

    int confirm;
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
}

void markFineAsPaidView(Violation violations[], int vCount, Member members[],
                        int mCount) {
    char violationID[50];
    printf("\n--- Mark Fine as Paid ---\n");
    printf("Enter Violation ID: ");
    scanf("%s", violationID);

    int vIndex = getViolationIndexById(violations, vCount, violationID);
    if (vIndex == -1) {
        printf("Error: Violation ID not found.\n");
        return;
    }

    if (violations[vIndex].isPaid == ALREADY_PAID) {
        printf("This violation is already paid.\n");
        return;
    }

    int confirm;
    inputYesNo(&confirm, "Confirm mark as paid? (1: Yes, 0: No): ");
    if (confirm) {
        violations[vIndex].isPaid = ALREADY_PAID;
        updateMemberTotalFine(members, mCount, violations, vCount,
                              violations[vIndex].studentID);
        saveViolations(violations, vCount);
        saveMembers(members, mCount);
        printf("Marked as paid successfully.\n");
    }
}

void displayViolationsByTimeRange(Violation violations[], int vCount) {

    Violation *results = (Violation *) malloc(sizeof(Violation) * vCount);
    int resultCount = 0;

    resultCount = listViolationsByTimeRange(violations, vCount, results);

    if (resultCount == 0) {
        printf("No violations found in the specified time range.\n");
        free(results);
        return;
    }
    
    displayViolationTableHeader();
    for (int i = 0; i < resultCount; i++) {
        displayViolationRow(&results[i]);
    }
    displayViolationTableFooter();

    free(results);
}

