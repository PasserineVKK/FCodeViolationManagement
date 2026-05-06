#include "../../include/view/violationView.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../include/consoleInput.h"
#include "../../include/report.h"
#include "../../include/utils.h"
#include "../../include/view/viewUtil.h"
#include "../../include/violation.h"
#include "../../include/member.h"

void displayViolationTableHeader() {
    printf(
        "\n┏━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━"
        "━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        "━━━━┳━━━━━━━━━━━━━━┓\n");
    printf(
        "┃ %-10s ┃ %-10s ┃ %-20s ┃ %-20s ┃ %-8s ┃ %-8s ┃ %-10s ┃ %-32s ┃ %-12s ┃\n",
        "ID", "Student", "Reason", "Time", "Fine", "Paid", "Penalty", "Note", "Pending");
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

void displayViolationRow(const Violation* v) {
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

    const char* paidStr = v->isPaid ? ((v->isPaid == ALREADY_PAID)? "Yes" : "Not have to pay") : "No";
    const char* penaltyStr = v->penalty ? "Kick" : "Financial";
    const char* pendingStr = v->owner->isPending ? "Pending" : "Resolved";

    printf(
        "┃ %-10s ┃ %-10s ┃ %-20s ┃ %-20s ┃ %-8.0f ┃ %-8s ┃ %-10s ┃ %-32s ┃ %-12s ┃\n",
        v->violationID, v->studentID, reasonStr, timeField, v->fine, paidStr, penaltyStr, v->note, pendingStr);
}

void displayViolationList(const Violation violations[], int vCount) {
    displayViolationTableHeader();
    for (int i = 0; i < vCount; i++) displayViolationRow(&violations[i]);
    displayViolationTableFooter();
}

void displayViolationByStudentId(const char* id, const ViolationList* violations) {
    displayViolationTableHeader();
    for (int i = 0; i < violations->count; i++) {
        if (strcmp(id, violations->data[i].studentID) == 0)
            displayViolationRow(&violations->data[i]);
    }
    displayViolationTableFooter();
}

void markFineAsPaidView(ViolationList* violations, MemberList* members) {
    char violationID[50];
    printf("\n--- Mark Fine as Paid ---\n");
    printf("Enter Violation ID: ");
    scanf("%s", violationID);

    int vIndex = getViolationIndexById(violations, violationID);
    if (vIndex == -1) {
        printf("Error: Violation ID not found.\n");
        return;
    }

    if (violations->data[vIndex].isPaid == ALREADY_PAID) {
        printf("This violation is already paid.\n");
        return;
    }

    if (violations->data[vIndex].isPaid == NOT_HAVE_TO_PAY) {
        printf("This violation is not have to pay\n");
        return;
    }

    int confirm;
    inputYesNo(&confirm, "Confirm mark as paid? (1: Yes, 0: No): ");
    if (confirm) {
        violations->data[vIndex].isPaid = ALREADY_PAID;
        updateMemberTotalFine(members, violations, violations->data[vIndex].studentID);
        saveViolations(violations);
        saveMembers(members);
        printf("Marked as paid successfully.\n");
    }
}

void displayViolationsByTimeRange(const ViolationList* violations) {
    // Allocate temporary memory for results based on current count
    Violation* results = (Violation*)malloc(sizeof(Violation) * violations->count);
    if (results == NULL) return;

    int resultCount = 0;

    // Assuming listViolationsByTimeRange is refactored to accept (const ViolationList*, Violation*)
    resultCount = listViolationsByTimeRange(violations->data, violations->count, results);


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

// 1.3 View unpaid fines for a member
void viewMyUnpaidFines(const char* myStudentID, const ViolationList* violations) {
    printf("\n==== Unpaid Fines ====\n");
    printf("Student ID: %s\n", myStudentID);

    const char* reasonNames[] = {
        "Not uniform",
        "Meeting absence",
        "Not join in Club activity",
        "Violence"
    };

    printf("\n┏━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┓\n");

    printf("┃ %-12s ┃ %-24s ┃ %-20s ┃ %-16s ┃\n",
        "Violation ID",
        "Reason",
        "Time",
        "Fine");

    printf("┣━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━┫\n");

    double total = 0.0;
    int found = 0;
    for (int i = 0; i < violations->count; i++) {
        const Violation* v = &violations->data[i];
        if (strcmp(v->studentID, myStudentID) != 0) continue;
        if (v->isPaid == ALREADY_PAID && v->isPaid == NOT_HAVE_TO_PAY) continue;

        char timeStr[20];
        getFormatTime(timeStr, sizeof(timeStr), v->violationTime);

        const char* reason =(v->reason >= 0 && v->reason <= 3)
                                ? reasonNames[v->reason]
                                : "Unknown";

        char fineStr[30];
        formatCurrency(v->fine, fineStr, sizeof(fineStr));

        printf("┃ %-12s ┃ %-24s ┃ %-20s ┃ %16s ┃\n",
            v->violationID, reason, timeStr, fineStr);

        total += v->fine;
        found++;
    }

    printf("┣━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━┫\n");

    if (found == 0) {

        printf("┃ %-78s ┃\n",
            "No unpaid fines found.");

    } else {

        char totalStr[30];

        formatCurrency(total, totalStr, sizeof(totalStr));

        printf("┃ %-61s  ┃ %-16s ┃\n",
            "Total Unpaid Fines",
            totalStr);
    }

    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┛\n");
}
