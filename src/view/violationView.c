#include "../../include/view/violationView.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../include/consoleInput.h"
#include "../../include/utils.h"
#include "../../include/view/viewUtil.h"

void displayViolationTableHeader() {
    printf(
        "\n┏━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━"
        "━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        "━━━━┳━━━━━━━━━━━━━━┓\n");
    printf(
        "┃ %-10s ┃ %-10s ┃ %-20s ┃ %-20s ┃ %-8s ┃ %-8s ┃ %-10s ┃ %-32s ┃ %-12s "
        "┃\n",
        "ID", "Student", "Reason", "Time", "Fine", "Paid", "Penalty", "Note",
        "Pending");
    printf(
        "┣━━━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━"
        "━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        "━━╋━━━━━━━━━━━━━━┫\n");
}

void displayViolationTableFooter() {
    printf(
        "┗━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━"
        "━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        "━━┻━━━━━━━━━━━━━━┛\n");
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

// 1.3 View unpaid fines for a member
void viewMyUnpaidFines(const char* myStudentID, Violation violations[],
                       int vCount) {
    printf("\n==== Unpaid Fines ====\n");
    printf("Student ID: %s\n", myStudentID);

    const char* reasonNames[] = {"Not uniform", "Meeting absence",
                                 "Not join in Club activity", "Violence"};
    printf("%-12s %-22s %-20s %s\n", "Violation ID", "Reason", "Time", "Fine");
    printf(
        "----------------------------------------------------------------------"
        "\n");
    double total = 0.0;
    int found = 0;
    for (int i = 0; i < vCount; i++) {
        Violation* v = &violations[i];
        if (strcmp(v->studentID, myStudentID) != 0) continue;
        if (v->isPaid != 0) continue;

        char timeStr[20];
        getFormatTime(timeStr, sizeof(timeStr), v->violationTime);

        const char* reason = (v->reason >= 0 && v->reason <= 3)
                                 ? reasonNames[v->reason]
                                 : "Unknown";

        char fineStr[30];
        formatCurrency(v->fine, fineStr, sizeof(fineStr));
        printf("%-12s %-22s %-20s %s\n", v->violationID, reason, timeStr,
               fineStr);

        total += v->fine;
        found++;
    }
    printf(
        "----------------------------------------------------------------------"
        "\n");
    if (found == 0) {
        printf("No unpaid fines found.\n");
    } else {
        char totalStr[30];
        formatCurrency(total, totalStr, sizeof(totalStr));
        printf("Total Unpaid Fines: %s\n", totalStr);
    }
    printf("========================\n");
}
