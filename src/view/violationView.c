#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/view/violationView.h"
#include "../include/view/viewUtil.h"

void displayViolationHeader()
{
    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-10s | %-20s | %-20s | %-8s | %-8s | %-10s | %-20s | %-10s |\n",
           "ID", "Student", "Reason", "Time", "Fine", "Paid", "Penalty", "Note", "Pending");
    printf("-------------------------------------------------------------------------------------------------------------\n");
}

void displayViolationFooter()
{
    printf("-------------------------------------------------------------------------------------------------------------\n");
}

void displayViolation(Violation *v)
{
    char timeField[30];

    getFormatTime(timeField, 30, v->violationTime);

    const char *reasonStr;
    switch (v->reason)
    {
    case 0:
        reasonStr = "Not uniform";
        break;
    case 1:
        reasonStr = "Meeting absence";
        break;
    case 2:
        reasonStr = "No Club activity";
        break;
    case 3:
        reasonStr = "Violence";
        break;
    default:
        reasonStr = "Unknown";
        break;
    }

    const char *paidStr = v->isPaid ? "Yes" : "No";
    const char *penaltyStr = v->pelnaty ? "Kick" : "Financial";
    const char *pendingStr = v->isPending ? "Pending" : "Not pending";

    printf("| %-10s | %-10s | %-20s | %-20s | %-8.2f | %-8s | %-10s | %-20s | %-10s |\n",
           v->violationID,
           v->studentID,
           reasonStr,
           timeField,
           v->fine,
           paidStr,
           penaltyStr,
           v->note,
           pendingStr);
}

void displayViolationList(Violation violations[], int size)
{
    displayViolationHeader();
    for (int i = 0; i < size; i++)
        displayViolation(&violations[i]);
    displayViolationFooter();
}

void displayViolationByStudentId(char *id, Violation violations[], int size)
{
    displayViolationHeader();
    for (int i = 0; i < size; i++)
        if (strcmp(id, violations[i].studentID) == 0)
            displayViolation(&violations[i]);
    displayViolationFooter();
}

void displayViolationsByPelnatyStatus(Violation violations[], int size, int pelnatyStatus)
{
    if (pelnatyStatus != PENALTY_FINANCIAL || pelnatyStatus != PENALTY_KICK)
    {
        return;
    }

    displayHeader();
    for (int i = 0; i < size; i++)
    {
        if (violations[i].pelnaty == pelnatyStatus)
        {
            displayViolation(&violations[i]);
        }
    }
    displayFooter();
}

void displayViolationsByReason(Violation violations[], int size, int reason)
{
    displayHeader();
    for (int i = 0; i < size; i++)
    {
        if (violations[i].reason == reason)
        {
            displayViolation(&violations[i]);
        }
    }
    displayFooter();
}

void displayViolationsByPaidStatus(Violation violations[], int size, int isPaid)
{
    displayHeader();
    for (int i = 0; i < size; i++)
    {
        if (violations[i].isPaid == isPaid)
        {
            displayViolation(&violations[i]);
        }
    }
    displayFooter();
}
