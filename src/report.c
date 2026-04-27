#include <stdio.h>
#include <string.h>
#include "../include/report.h"
#include "../include/member.h"
#include "../include/violation.h"
#include "../include/utils.h"
#include "../include/consoleInput.h"
#include "../include/view/viewUtil.h"

// 2.7 Show fine statistics by team
void showFineStatsByTeam(Member members[], int mCount, Violation violations[], int vCount)
{
    const char *teamNames[] = {"Academic", "Planning", "HR", "Media"};

    double paidByTeam[4] = {0};
    double unpaidByTeam[4] = {0};

    for (int i = 0; i < vCount; i++)
    {
        Violation *v = &violations[i];

        int mIdx = searchMemberByIdInM(members, mCount, v->studentID);
        if (mIdx == -1)
            continue;

        int team = members[mIdx].team;
        if (team < 0 || team > 3)
            continue;

        if (v->isPaid)
        {
            paidByTeam[team] += v->fine;
        }
        else
        {
            unpaidByTeam[team] += v->fine;
        }
    }

    printf("\n===== STATISTICS FINE BY TEAM =====\n");
    printf("%-12s %18s %18s %18s\n", "Team", "Paid", "Unpaid", "Total");
    printf("--------------------------------------------------------------\n");

    double grandPaid = 0, grandUnpaid = 0;

    for (int i = 0; i < 4; i++)
    {
        char paidStr[30], unpaidStr[30], totalStr[30];
        formatCurrency(paidByTeam[i], paidStr, sizeof(paidStr));
        formatCurrency(unpaidByTeam[i], unpaidStr, sizeof(unpaidStr));
        formatCurrency(paidByTeam[i] + unpaidByTeam[i], totalStr, sizeof(totalStr));

        printf("%-12s %18s %18s %18s\n",
               teamNames[i], paidStr, unpaidStr, totalStr);

        grandPaid += paidByTeam[i];
        grandUnpaid += unpaidByTeam[i];
    }

    printf("--------------------------------------------------------------\n");
    char gPaid[30], gUnpaid[30], gTotal[30];
    formatCurrency(grandPaid, gPaid, sizeof(gPaid));
    formatCurrency(grandUnpaid, gUnpaid, sizeof(gUnpaid));
    formatCurrency(grandPaid + grandUnpaid, gTotal, sizeof(gTotal));
    printf("%-12s %18s %18s %18s\n",
           "TOTAL", gPaid, gUnpaid, gTotal);
    printf("\n");
}

void ensureNotificationCapacity(Notification **list, int *count, int *capacity)
{
    if (*count >= *capacity)
    {
        *capacity = (*capacity == 0) ? 10 : (*capacity * 2);
        Notification *temp = realloc(*list, (*capacity) * sizeof(Notification));
        if (temp == NULL)
        {
            fprintf(stderr, "Allocation failed\n");
            return;
        }
        *list = temp;
    }
}

void createNotification(Notification **list, int *count, int *capacity,
                        const char *receiverID, int type, const char *message, time_t deleteTime)
{
    ensureNotificationCapacity(list, count, capacity);
    Notification *n = &((*list)[*count]);
    memset(n->id, 0, sizeof(n->id));
    n->id[0] = *count + 1;
    n->type = type;
    strncpy(n->studentId, receiverID, 10);
    n->studentId[9] = '\0';
    strncpy(n->content, message, MAX_MESSAGE_LENGTH);
    n->content[MAX_MESSAGE_LENGTH - 1] = '\0';
    n->deleteTime = deleteTime;
    (*count)++;
}

Notification *findNotificationById(const char *notificationId, Notification *list, int count)
{
    int idVal = atoi(notificationId);
    for (int i = 0; i < count; i++)
    {
        if (list[i].id[0] == idVal)
        {
            return &list[i];
        }
    }
    return NULL;
}

void updateNotification(Notification *n,
                        const char *receiverID, int type, const char *message, time_t deleteTime)
{
    if (n == NULL)
        return;
    n->type = type;
    strncpy(n->studentId, receiverID, 10);
    n->studentId[9] = '\0';
    strncpy(n->content, message, MAX_MESSAGE_LENGTH);
    n->content[MAX_MESSAGE_LENGTH - 1] = '\0';
    n->deleteTime = deleteTime;
}

void deleteNotification(Notification *n, Notification **list, int *count)
{
    if (n == NULL || list == NULL || *list == NULL || *count <= 0)
        return;

    for (int i = 0; i < *count; i++)
    {
        if (&((*list)[i]) == n)
        {
            for (int j = i; j < *count - 1; j++)
            {
                (*list)[j] = (*list)[j + 1];
            }
            (*count)--;
            return;
        }
    }
}
