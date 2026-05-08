#include "../include/report.h"

#include <stdio.h>
#include <string.h>

#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/utils.h"
#include "../include/view/viewUtil.h"
#include "../include/violation.h"
#include "../include/validate.h"

static Notification *notifications;
static int count;
static int capacity;

static int maxCount;

#define MAX_NOTIFICATIONS 1000

// Validate
int isValidateNotification(Notification *n)
{
    if (isBlank(n->content))
        return 0;
    if (n->type == ADMIN_WARNING && !isValidStudentID(n->memberId))
        return 0;
    else if (n->type != ADMIN_WARNING && (isBlank(n->memberId) || !isValidStudentID(n->memberId) || !(n->memberId == NULL)))
        return 0;
    return 1;
}

int listViolationsByTimeRange(Violation violations[], int vCount,
                              Violation results[]);

void initNotificationList()
{
    notifications = malloc(sizeof(Notification) * MAX_NOTIFICATIONS);
    capacity = MAX_NOTIFICATIONS;
    count = 0;

    loadFromFile(NOTIFICATION_PATH, notifications, sizeof(Notification),
                 MAX_NOTIFICATIONS, &count);
    maxCount = count;
}

void saveNotification()
{
    saveToFile(NOTIFICATION_PATH, notifications, sizeof(Notification), count);
}

// 2.7 Show fine statistics by team
void showFineStatsByTeam(MemberList *members, ViolationList *violations)
{
    const char *teamNames[] = {"Academic", "Planning", "HR", "Media"};

    double paidByTeam[4] = {0};
    double unpaidByTeam[4] = {0};

    for (int i = 0; i < violations->count; i++)
    {
        Violation *v = &violations->data[i];

        int mIndex = searchMemberByIdInM(members, v->owner->studentID);
        if (mIndex == -1)
            continue;

        int team = members->data[mIndex].team;
        if (team < 0 || team > 3)
            continue;

        if (v->isPaid == 1)
        {
            paidByTeam[team] += v->fine;
        }
        else if (v->isPaid == 0)
        {
            unpaidByTeam[team] += v->fine;
        }
    }

    uiTableTitle("STATISTIC FINE BY TEAM");
    printf(
        "\n┏━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━"
        "━━━━┓\n");
    printf("┃ %-12s ┃ %16s ┃ %16s ┃ %16s ┃\n", "Team", "Paid", "Unpaid",
           "Paid + Unpaid");

    printf(
        "┣━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━"
        "━━┫\n");

    double grandPaid = 0, grandUnpaid = 0;

    for (int i = 0; i < 4; i++)
    {
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

void displayNotifications(int type)
{
    for (int i = 0; i < count; i++)
        if (notifications[i].type == type)
            displaySingleNotification(&notifications[i]);
}

void displayWarning(const char *memberId)
{
    for (int i = 0; i < count; i++)
        if (strcmp(notifications[i].memberId, memberId) == 0)
            displaySingleNotification(&notifications[i]);
}

void displayNotificationByMemberID(const char *studentId, int type)
{
    for (int i = 0; i < count; i++)
    {
        Notification *n = &notifications[i];
        if (strcmp(n->memberId, studentId) == 0 && n->type == type)
            displaySingleNotification(n);
    }
}

void displayGlobalNotification()
{
    for (int i = 0; i < count; i++)
        if (notifications[i].type == GLOBAL_NOTICE)
            displaySingleNotification(&notifications[i]);
}

void displayNotificationList()
{
    for (int i = 0; i < count; i++)
    {
        displaySingleNotification(&notifications[i]);
        printf("Id: %s\n", notifications[i].id);
    }
}

void displaySingleNotification(Notification *n)
{
    char timeBuf[30];
    getFormatTime(timeBuf, 30, n->deleteTime);

    switch (n->type)
    {
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

void ensureNotificationCapacity()
{
    if (count >= capacity)
    {
        capacity = (capacity == 0) ? 10 : (capacity * 2);
        Notification *temp = realloc(notifications, (capacity) * sizeof(Notification));
        if (temp == NULL)
        {
            fprintf(stderr, "Allocation failed\n");
            return;
        }
        notifications = temp;
    }
}

Notification *createNotification(const char *receiverID, int type, const char *message, time_t deleteTime, int isSave)
{
    ensureNotificationCapacity();
    Notification n;

    sprintf(n.id, "%06d", maxCount + 1);
    n.type = type;
    n.deleteTime = deleteTime;
    n.create_time = time(NULL);
    strcpy(n.content, message);
    if (!(receiverID == NULL))
        strcpy(n.memberId, receiverID);
    notifications[count] = n;
    count++;
    if (isSave)
    {
        maxCount++;
        saveNotification();
    }
    return &notifications[count - 1];
}

Notification *findNotificationById(const char *notificationId)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(notificationId, notifications[i].id) == 0)
        {
            return &notifications[i];
        }
    }
    return NULL;
}

void updateNotification(Notification *n, const char *receiverID, int type, const char *message, time_t deleteTime)
{
    if (n == NULL)
        return;
    n->type = type;
    strncpy(n->memberId, receiverID, 10);
    n->memberId[9] = '\0';
    strncpy(n->content, message, MAX_MESSAGE_LENGTH);
    n->content[MAX_MESSAGE_LENGTH - 1] = '\0';
    n->deleteTime = deleteTime;
    saveNotification();
}

void deleteNotification(Notification *n)
{
    if (n == NULL || notifications == NULL || count <= 0)
        return;

    for (int i = 0; i < count; i++)
    {
        if (&notifications[i] == n)
        {
            for (int j = i; j < count - 1; j++)
            {
                notifications[j] = notifications[j + 1];
            }
            count--;
            break;
        }
    }

    saveNotification();
}

void deleteNotificationByMemberId(const char *memberId)
{
    for (int i = 0; i < count; i++)
        if (strcmp(memberId, notifications[i].memberId) == 0)
        {
            for (int j = 0; j < i - 1; j++)
                notifications[j] = notifications[j + 1];
            count--;
        }

    saveNotification();
}

void autoDeleteOutDateNotification()
{
    time_t now = time(NULL);
    for (int i = 0; i < count; i++)
        if (now > notifications[i].deleteTime)
        {
            for (int j = 0; j < i - 1; j++)
                notifications[j] = notifications[j + 1];
            count--;
        }

    saveNotification();
}

Notification *notifyAdmin(const char *content, const char *adminId, int isSave)
{
    Notification *n = createNotification(adminId, ADMIN_NOTICE, content, time(NULL) + BASE_DELETE_TIME, isSave);
    displaySingleNotification(n);
}

Notification *warningMember(const char *content, const char *memberId, int isSave) { createNotification(memberId, ADMIN_WARNING, content, time(NULL) + BASE_DELETE_TIME, isSave); }

Notification *globalNotification(const char *content) { createNotification(NULL, GLOBAL_NOTICE, content, time(NULL) + BASE_DELETE_TIME, 1); }

void quickNotification(const char *content) { displaySingleNotification(createNotification(NULL, GLOBAL_NOTICE, content, time(NULL), NOT_SAVE)); }

void freeNotificationList()
{
    if (notifications == NULL)
        return;
    free(notifications);
}

int listViolationsByTimeRange(Violation violations[], int vCount,
                              Violation results[])
{
    time_t start, end;
    inputTimeRange(&start, &end, "Enter time range (YYYY-MM-DD HH:MM): ");

    int foundCount = 0;
    for (int i = 0; i < vCount; i++)
    {
        if (violations[i].violationTime >= start &&
            violations[i].violationTime <= end)
        {
            results[foundCount++] = violations[i];
        }
    }
    return foundCount;
}

// Advanced feature: Export violation report to .txt file
// Includes timestamp, summary by team, and members with outstanding fines
void exportViolationReportToFile(MemberList *members, ViolationList *violations) {
    if (members == NULL || violations == NULL) {
        printf("\033[1;31m[ERROR] Invalid data!\033[0m\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char filename[100];
    strftime(filename, sizeof(filename), "FCODE_ViolationReport_%Y%m%d_%H%M.txt", timeinfo);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("\033[1;31m[ERROR] Cannot create report file!\033[0m\n");
        return;
    }

    char timeStr[50];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(file, "VIOLATION REPORT\n");
    fprintf(file, "Generated at: %s\n\n", timeStr);

    const char* teamNames[] = {"Academic", "Planning", "HR", "Media"};
    double paidByTeam[4] = {0};
    double unpaidByTeam[4] = {0};
    int violationCountByTeam[4] = {0};
    int memberCountByTeam[4] = {0};

    for (int i = 0; i < violations->count; i++) {
        Violation* v = &violations->data[i];
        int mIndex = searchMemberByIdInM(members, v->owner->studentID);
        if (mIndex == -1) continue;

        int team = members->data[mIndex].team;
        if (team < 0 || team > 3) continue;

        violationCountByTeam[team]++;
        if (v->isPaid == 1) {
            paidByTeam[team] += v->fine;
        } else if (v->isPaid == 0) {
            unpaidByTeam[team] += v->fine;
        }
    }

    for (int team = 0; team < 4; team++) {
        for (int i = 0; i < members->count; i++) {
            int hasViolation = 0;
            if (members->data[i].team != team) continue;

            for (int v = 0; v < violations->count; v++) {
                if (strcmp(violations->data[v].owner->studentID,
                           members->data[i].studentID) == 0) {
                    hasViolation = 1;
                    break;
                }
            }

            if (hasViolation) memberCountByTeam[team]++;
        }
    }

    double grandPaid = 0;
    double grandUnpaid = 0;
    int grandViolations = 0;

        fprintf(file, "SUMMARY BY DEPARTMENT\n");
        fprintf(file, "+----------------+------------+-------------------------+------------------+------------------+------------------+\n");
        fprintf(file, "| %-14s | %-10s | %-23s | %-16s | %-16s | %-16s |\n",
            "Department", "Violations", "Members with Violations", "Paid", "Unpaid", "Total");
        fprintf(file, "+----------------+------------+-------------------------+------------------+------------------+------------------+\n");

    for (int i = 0; i < 4; i++) {
        char paidStr[25], unpaidStr[25], totalStr[25];
        formatCurrency(paidByTeam[i], paidStr, sizeof(paidStr));
        formatCurrency(unpaidByTeam[i], unpaidStr, sizeof(unpaidStr));
        formatCurrency(paidByTeam[i] + unpaidByTeam[i], totalStr, sizeof(totalStr));

        fprintf(file, "| %-14s | %10d | %23d | %-16s | %-16s | %-16s |\n",
                teamNames[i], violationCountByTeam[i], memberCountByTeam[i],
                paidStr, unpaidStr, totalStr);

        grandPaid += paidByTeam[i];
        grandUnpaid += unpaidByTeam[i];
        grandViolations += violationCountByTeam[i];
    }

    char gPaid[25], gUnpaid[25], gTotal[25];
    formatCurrency(grandPaid, gPaid, sizeof(gPaid));
    formatCurrency(grandUnpaid, gUnpaid, sizeof(gUnpaid));
    formatCurrency(grandPaid + grandUnpaid, gTotal, sizeof(gTotal));

            fprintf(file, "+----------------+------------+-------------------------+------------------+------------------+------------------+\n");
            fprintf(file, "| %-14s | %10d | %23s | %-16s | %-16s | %-16s |\n",
            "GRAND TOTAL", grandViolations, "", gPaid, gUnpaid, gTotal);
                fprintf(file, "+----------------+------------+-------------------------+------------------+------------------+------------------+\n");

    fprintf(file, "MEMBERS WITH OUTSTANDING FINES\n");
                fprintf(file, "+------------+------------------------+--------------+------------+------------------+\n");
                fprintf(file, "| %-10s | %-22s | %-12s | %-10s | %-16s |\n",
            "Student ID", "Full Name", "Department", "Violations", "Fine Amount");
                fprintf(file, "+------------+------------------------+--------------+------------+------------------+\n");

    int outstandingCount = 0;
    double totalOutstanding = 0;

    for (int team = 0; team < 4; team++) {
        int membersInTeam = 0;

        for (int i = 0; i < members->count; i++) {
            Member* m = &members->data[i];
            if (m->team != team) continue;

            double unpaidAmount = 0;
            int unpaidCount = 0;

            for (int v = 0; v < violations->count; v++) {
                Violation* viol = &violations->data[v];
                if (strcmp(viol->studentID, m->studentID) == 0 && viol->isPaid == 0) {
                    unpaidAmount += viol->fine;
                    unpaidCount++;
                }
            }

            if (unpaidAmount > 0) {
                char fineStr[25];
                formatCurrency(unpaidAmount, fineStr, sizeof(fineStr));
                fprintf(file, "| %-10s | %-22s | %-12s | %10d | %-16s |\n",
                    m->studentID, m->fullName, teamNames[team], unpaidCount, fineStr);

                totalOutstanding += unpaidAmount;
                outstandingCount++;
                membersInTeam++;
            }
        }

        if (membersInTeam == 0) {
                continue;
        }
        }

            fprintf(file, "+------------+------------------------+--------------+------------+------------------+\n");

    char totalOutstandingStr[25];
    formatCurrency(totalOutstanding, totalOutstandingStr, sizeof(totalOutstandingStr));

    fprintf(file, "\nTOTAL MEMBERS WITH OUTSTANDING FINES: %d\n", outstandingCount);
    fprintf(file, "TOTAL OUTSTANDING AMOUNT: %s\n", totalOutstandingStr);
    fprintf(file, "TOTAL VIOLATIONS: %d\n", grandViolations);

    fclose(file);

    printf("\033[1;32m[SUCCESS] Report exported to: %s\033[0m\n", filename);
}
void displayNotificationManagerMenu()
{
    printf("\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"
           "\n┃        NOTIFICATION MANAGER MENU             ┃"
           "\n┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫"
           "\n┃  1. Add new notification                     ┃"
           "\n┃  2. Update notification                      ┃"
           "\n┃  3. Delete notification                      ┃"
           "\n┃  4. Display all notification                 ┃"
           "\n┃  5. Out this service                         ┃"
           "\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}

void createNotificationView()
{
    int option;
    do
    {
        int type;
        inputIntegerInRange(&type, 0, 2,
                            "Enter type of notification, global(0), notice(1), warning(2): ");
        char content[200];
        inputString(content, sizeof(content), "Enter notification content: ");

        // Content must not be blank
        if (isBlank(content))
        {
            notifyAdmin("Content must not be blank!", NULL, NOT_SAVE);
            continue;
        }

        char memberId[16] = "";
        if (type == ADMIN_NOTICE || type == ADMIN_WARNING)
        {
            inputString(memberId, sizeof(memberId), "Enter member id: ");
        }

        int valid = 1;
        if (type == ADMIN_NOTICE &&
            !isBlank(memberId) && !isValidStudentID(memberId))
        {
            notifyAdmin("Invalid member id for ADMIN_NOTICE!", NULL, NOT_SAVE);
            valid = 0;
        }
        else if (type == ADMIN_WARNING &&
                 !isValidStudentID(memberId))
        {
            notifyAdmin("Invalid member id for ADMIN_WARNING!", NULL, NOT_SAVE);
            valid = 0;
        }
        else if (type == GLOBAL_NOTICE &&
                 !isBlank(memberId))
        {
            notifyAdmin("Global notification must not have member id!", NULL, NOT_SAVE);
            valid = 0;
        }

        if (valid)
        {
            int confirm = 1;
            if (type == GLOBAL_NOTICE)
            {
                inputYesNo(&confirm, "Are you sure to create this global notification? Yes(1), No(0): ");
            }
            if (confirm)
            {
                createNotification((isBlank(memberId) ? NULL : memberId),
                                   type, content, time(NULL) + BASE_DELETE_TIME, 1);
                notifyAdmin("Notification created successfully!", NULL, NOT_SAVE);
            }
        }

        inputYesNo(&option, "\nDo you want to add another notification? Yes(1), No(0): ");
        if (option)
            clearScreen();
    } while (option);
}

void updateNotificationView()
{
    int option;
    do
    {
        displayNotificationList();
        char id[16];
        inputString(id, sizeof(id), "\nEnter notification id: ");
        Notification *n = findNotificationById(id);

        if (n == NULL)
        {
            notifyAdmin("Notification not found!", NULL, NOT_SAVE);
        }
        else
        {
            Notification updateN;
            inputIntegerInRange(&updateN.type, 0, 2,
                                "Enter type of notification, global(0), notice(1), warning(2): ");
            inputString(updateN.content, sizeof(updateN.content), "Enter notification content: ");
            inputString(updateN.memberId, sizeof(updateN.memberId), "Enter member id: ");

            // Validate once
            int valid = 1;
            if (isBlank(updateN.content))
            {
                notifyAdmin("Content must not be blank!", NULL, NOT_SAVE);
                valid = 0;
            }
            else if (updateN.type == ADMIN_NOTICE &&
                     !isBlank(updateN.memberId) && !isValidStudentID(updateN.memberId))
            {
                notifyAdmin("Invalid member id for ADMIN_NOTICE!", NULL, NOT_SAVE);
                valid = 0;
            }
            else if (updateN.type == ADMIN_WARNING &&
                     !isValidStudentID(updateN.memberId))
            {
                notifyAdmin("Invalid member id for ADMIN_WARNING!", NULL, NOT_SAVE);
                valid = 0;
            }
            else if (updateN.type == GLOBAL_NOTICE &&
                     !isBlank(updateN.memberId))
            {
                notifyAdmin("Global notification must not have member id!", NULL, NOT_SAVE);
                valid = 0;
            }

            if (valid)
            {
                int confirm;
                inputYesNo(&confirm, "Are you sure to update this notification? Yes(1), No(0): ");
                if (confirm)
                {
                    updateNotification(n, updateN.memberId, updateN.type, updateN.content, n->deleteTime);
                    notifyAdmin("Update notification successfully!", NULL, NOT_SAVE);
                }
            }
        }

        inputYesNo(&option, "\nDo you want to update another notification? Yes(1), No(0): ");
        if (option)
            clearScreen();
    } while (option);
}

void deleteNotificationView()
{
    int option;
    do
    {
        displayNotificationList();
        char removeId[6];
        inputString(removeId, 6, "\nEnter remove notification id: ");
        Notification *foundNotification = findNotificationById(removeId);
        if (foundNotification != NULL)
        {
            int confirm;
            inputYesNo(&confirm, "Are you sure to delete this notification: ");
            if (confirm)
            {
                deleteNotification(foundNotification);
                notifyAdmin("Notification is deleted!!", NULL, NOT_SAVE);
            }
        }
        else
        {
            notifyAdmin("Can not find this notification!!", NULL, NOT_SAVE);
        }

        inputYesNo(&option, "\nDo you want to delete another notification: ");
        if (option)
        {
            clearScreen();
        }
    } while (option);
}
