#include "../include/report.h"

#include <stdio.h>
#include <string.h>

#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/utils.h"
#include "../include/view/viewUtil.h"
#include "../include/violation.h"

static Notification* notifications;
static int count;
static int capacity;

#define MAX_NOTIFICATIONS 1000

int listViolationsByTimeRange(Violation violations[], int vCount,
                              Violation results[]);

void initNotificationList() {
    notifications = malloc(sizeof(Notification) * MAX_NOTIFICATIONS);
    capacity = MAX_NOTIFICATIONS;
    count = 0;

    loadFromFile(NOTIFICATION_PATH, notifications, sizeof(Notification),
                 MAX_NOTIFICATIONS, &count);
}

void saveNotification() {
    saveToFile(NOTIFICATION_PATH, notifications, sizeof(Notification), count);
}

// 2.7 Show fine statistics by team
void showFineStatsByTeam(MemberList *members, ViolationList *violations) {
    const char* teamNames[] = {"Academic", "Planning", "HR", "Media"};

    double paidByTeam[4] = {0};
    double unpaidByTeam[4] = {0};

    for (int i = 0; i < violations->count; i++) {
        Violation* v = &violations->data[i];

        int mIndex = searchMemberByIdInM(members, v->owner->studentID);
        if (mIndex == -1) continue;

        int team = members->data[mIndex].team;
        if (team < 0 || team > 3) continue;

        if (v->isPaid == 1) {
            paidByTeam[team] += v->fine;
        } else if (v->isPaid == 0) {
            unpaidByTeam[team] += v->fine;
        }
    }

    printf("\n===== STATISTICS FINE BY TEAM =====\n");
    printf(
        "\n┏━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━"
        "━━━━┓\n");
    printf("┃ %-12s ┃ %16s ┃ %16s ┃ %16s ┃\n", "Team", "Paid", "Unpaid",
           "Paid + Unpaid");

    printf(
        "┣━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━"
        "━━┫\n");

    double grandPaid = 0, grandUnpaid = 0;

    for (int i = 0; i < 4; i++) {
        char paidStr[30], unpaidStr[30], totalStr[30];

        formatCurrency(paidByTeam[i], paidStr, sizeof(paidStr));
        formatCurrency(unpaidByTeam[i], unpaidStr, sizeof(unpaidStr));
        formatCurrency(paidByTeam[i] + unpaidByTeam[i], totalStr,
                       sizeof(totalStr));

        printf("┃ %-12s ┃ %16s ┃ %16s ┃ %16s ┃\n", teamNames[i], paidStr,
               unpaidStr, totalStr);

        grandPaid += paidByTeam[i];
        grandUnpaid += unpaidByTeam[i];
    }

    printf(
        "┣━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━"
        "━━┫\n");

    char gPaid[30], gUnpaid[30], gTotal[30];

    formatCurrency(grandPaid, gPaid, sizeof(gPaid));
    formatCurrency(grandUnpaid, gUnpaid, sizeof(gUnpaid));
    formatCurrency(grandPaid + grandUnpaid, gTotal, sizeof(gTotal));

    printf("┃ %-12s ┃ %16s ┃ %16s ┃ %16s ┃\n", "TOTAL", gPaid, gUnpaid, gTotal);

    printf(
        "┗━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━"
        "━━┛\n\n");
}

void displayNotifications(int type) {
    for (int i = 0; i < count; i++)
        if (notifications[i].type == type)
            displaySingleNotification(&notifications[i]);
}

void displayWarning(const char* memberId) {
    for (int i = 0; i < count; i++)
        if (strcmp(notifications[i].memberId, memberId) == 0)
            displaySingleNotification(&notifications[i]);
}

void displayNotificationByMemberID(const char* studentId, int type) {
    for (int i = 0; i < count; i++) {
        Notification* n = &notifications[i];
        if (strcmp(n->memberId, studentId) == 0) {
            if (type != IGNORE_NOTI_TYPE && n->type == type)
                displaySingleNotification(n);
            else
                displaySingleNotification(n);
        }
    }
}

void displayGlobalNotification() {
    for (int i = 0; i < count; i++)
        if (notifications[i].type == GLOBAL_NOTICE)
            displaySingleNotification(&notifications[i]);
}

void displayNotificationList() {
    for (int i = 0; i < count; i++) {
        displaySingleNotification(&notifications[i]);
        printf("Id: %s\n", notifications[i].id);
    }
}

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

    char deleteTime[20];
    getFormatTime(deleteTime, 20, n->deleteTime);
    printf("Delete time: %s\n", deleteTime);
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
                                 const char* message, time_t deleteTime,
                                 int isSave) {
    ensureNotificationCapacity();
    Notification n;

    sprintf(n.id, "%06d", count + 1);
    n.type = type;
    n.deleteTime = deleteTime;
    n.create_time = time(NULL);
    strcpy(n.content, message);
    if (!(receiverID == NULL)) strcpy(n.memberId, receiverID);
    notifications[count] = n;
    count++;
    if (isSave) saveNotification();
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
    saveNotification();
}

void deleteNotification(Notification* n) {
    if (n == NULL || notifications == NULL || count <= 0) return;

    for (int i = 0; i < count; i++)
        if (&notifications[i] == n) {
            for (int j = 0; j < i - 1; j++)
                notifications[j] = notifications[j + 1];
            count--;
        }

    saveNotification();
}

void deleteNotificationByMemberId(const char* memberId) {
    for (int i = 0; i < count; i++)
        if (strcmp(memberId, notifications[i].memberId) == 0) {
            for (int j = 0; j < i - 1; j++)
                notifications[j] = notifications[j + 1];
            count--;
        }

    saveNotification();
}

void autoDeleteOutDateNotification() {
    time_t now = time(NULL);
    for (int i = 0; i < count; i++)
        if (now > notifications[i].deleteTime) {
            for (int j = 0; j < i - 1; j++)
                notifications[j] = notifications[j + 1];
            count--;
        }

    saveNotification();
}

Notification* notifyAdmin(const char* content, const char* adminId,
                          int isSave) {
    Notification* n = createNotification(adminId, ADMIN_NOTICE, content,
                                         time(NULL) + BASE_DELETE_TIME, isSave);
    displaySingleNotification(n);
}

Notification* warningMember(const char* content, const char* memberId,
                            int isSave) {
    createNotification(memberId, ADMIN_WARNING, content,
                       time(NULL) + BASE_DELETE_TIME, isSave);
}

Notification* globalNotification(const char* content) {
    createNotification(NULL, GLOBAL_NOTICE, content,
                       time(NULL) + BASE_DELETE_TIME, 1);
}

void quickNotification(const char* content) {
    displaySingleNotification(
        createNotification(NULL, GLOBAL_NOTICE, content, time(NULL), NOT_SAVE));
}

void freeNotificationList() {
    if (notifications == NULL) return;
    free(notifications);
}

int listViolationsByTimeRange(Violation violations[], int vCount,
                              Violation results[]) {
    time_t start, end;
    inputTimeRange(&start, &end, "Enter time range (YYYY-MM-DD HH:MM): ");

    int foundCount = 0;
    for (int i = 0; i < vCount; i++) {
        if (violations[i].violationTime >= start &&
            violations[i].violationTime <= end) {
            results[foundCount++] = violations[i];
        }
    }
    return foundCount;
}