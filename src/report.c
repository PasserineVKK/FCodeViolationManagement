#include "../include/report.h"

#include <stdio.h>
#include <string.h>

#include "../include/consoleInput.h"
#include "../include/member.h"
#include "../include/utils.h"
#include "../include/view/viewUtil.h"
#include "../include/violation.h"

static Notification* notifications;
static int count;
static int capacity;

void initNotificationList(int maxSize) {
    notifications = malloc(sizeof(Notification) * maxSize);
    capacity = maxSize;
    count = 0;
}

// 2.7 Show fine statistics by team
void showFineStatsByTeam(Member members[], int mCount, Violation violations[],
                         int vCount) {
    const char* teamNames[] = {"Academic", "Planning", "HR", "Media"};

    double paidByTeam[4] = {0};
    double unpaidByTeam[4] = {0};

    for (int i = 0; i < vCount; i++) {
        Violation* v = &violations[i];

        int mIdx = searchMemberByIdInM(members, mCount, v->studentID);
        if (mIdx == -1) continue;

        int team = members[mIdx].team;
        if (team < 0 || team > 3) continue;

        if (v->isPaid) {
            paidByTeam[team] += v->fine;
        } else {
            unpaidByTeam[team] += v->fine;
        }
    }

    printf("\n===== STATISTICS FINE BY TEAM =====\n");
    printf("%-12s %18s %18s %18s\n", "Team", "Paid", "Unpaid", "Total");
    printf("--------------------------------------------------------------\n");

    double grandPaid = 0, grandUnpaid = 0;

    for (int i = 0; i < 4; i++) {
        char paidStr[30], unpaidStr[30], totalStr[30];
        formatCurrency(paidByTeam[i], paidStr, sizeof(paidStr));
        formatCurrency(unpaidByTeam[i], unpaidStr, sizeof(unpaidStr));
        formatCurrency(paidByTeam[i] + unpaidByTeam[i], totalStr,
                       sizeof(totalStr));

        printf("%-12s %18s %18s %18s\n", teamNames[i], paidStr, unpaidStr,
               totalStr);

        grandPaid += paidByTeam[i];
        grandUnpaid += unpaidByTeam[i];
    }

    printf("--------------------------------------------------------------\n");
    char gPaid[30], gUnpaid[30], gTotal[30];
    formatCurrency(grandPaid, gPaid, sizeof(gPaid));
    formatCurrency(grandUnpaid, gUnpaid, sizeof(gUnpaid));
    formatCurrency(grandPaid + grandUnpaid, gTotal, sizeof(gTotal));
    printf("%-12s %18s %18s %18s\n", "TOTAL", gPaid, gUnpaid, gTotal);
    printf("\n");
}

void displayNotifications() {}

void displaySingleNotification(Notification* n) {
    char timeBuf[30];
    getFormatTime(timeBuf, 30, n->deleteTime);

    switch (n->type) {
        case GLOBAL_NOTICE:
            printf("\033[1;32m[GLOBAL NOTICE]\033[0m [%s]\n", n->content);
            break;
        case ADMIN_NOTICE:
            printf("\033[1;34m[ADMIN NOTICE]\033[0m [%s]\n", n->content);
            break;
        case ADMIN_WARNING:
            printf("\033[1;31m[ADMIN WARNING]\033[0m [%s]\n", n->content);
            break;
        default:
            printf("[UNKNOWN] [%s]\n", n->content);
            break;
    }
}

void ensureNotificationCapacity() {
    if (count >= capacity) {
        capacity = (capacity == 0) ? 10 : (capacity * 2);
        Notification* temp =
            realloc(notifications, (capacity) * sizeof(Notification));
        if (temp == NULL) {
            fprintf(stderr, "Allocation failed\n");
            return;
        }
        notifications = temp;
    }
}

Notification* createNotification(const char* receiverID, int type,
                                 const char* message, time_t deleteTime) {
    ensureNotificationCapacity();
    Notification n;

    sprintf(n.id, "%06d", count + 1);
    n.type = type;
    n.deleteTime = deleteTime;
    strcpy(n.content, message);
    if (!(receiverID == NULL)) strcpy(n.memberId, receiverID);
    notifications[count] = n;
    displaySingleNotification(&notifications[count]);
    count++;
    return &notifications[count - 1];
}

Notification* findNotificationById(const char* notificationId) {
    for (int i = 0; i < count; i++) {
        if (strcmp(notificationId, notifications[i].id)) {
            return &notifications[i];
        }
    }
    return NULL;
}

void updateNotification(Notification* n, const char* receiverID, int type,
                        const char* message, time_t deleteTime) {
    if (n == NULL) return;
    n->type = type;
    strncpy(n->memberId, receiverID, 10);
    n->memberId[9] = '\0';
    strncpy(n->content, message, MAX_MESSAGE_LENGTH);
    n->content[MAX_MESSAGE_LENGTH - 1] = '\0';
    n->deleteTime = deleteTime;
}

void deleteNotification(Notification* n) {
    if (n == NULL || notifications == NULL || notifications == NULL ||
        count <= 0)
        return;

    for (int i = 0; i < count; i++) {
        if (&((notifications)[i]) == n) {
            for (int j = i; j < count - 1; j++) {
                (notifications)[j] = (notifications)[j + 1];
            }
            (count)--;
            return;
        }
    }
}

void notifyAdmin(const char* content, const char* adminId) {
    Notification* n = createNotification(adminId, ADMIN_NOTICE, content,
                                         time(NULL) + BASE_DELETE_TIME);
    displaySingleNotification(n);
}

void warningMember(const char* content, const char* memberId) {
    createNotification(memberId, ADMIN_WARNING, content,
                       time(NULL) + BASE_DELETE_TIME);
}

void globalNotification(const char* content) {
    createNotification(NULL, GLOBAL_NOTICE, content,
                       time(NULL) + BASE_DELETE_TIME);
}

void freeNotificationList() {
    if (notifications == NULL) return;
    free(notifications);
}