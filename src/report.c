#include "../include/report.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

// Validates whether a notification contains a usable target and message.
int isValidateNotification(Notification *n)
{
    if (isBlank(n->content))
        return 0;
    if (n->type == ADMIN_WARNING && !isValidStudentID(n->memberId))
        return 0;
    else if (n->type == ADMIN_NOTICE && !isBlank(n->memberId) && !isValidStudentID(n->memberId))
        return 0;
    else if (n->type == GLOBAL_NOTICE && !isBlank(n->memberId))
        return 0;
    return 1;
}

int listViolationsByTimeRange(Violation violations[], int vCount,
                              Violation results[]);

// Allocates the notification list and loads any saved notifications from disk.
void initNotificationList()
{
    notifications = malloc(sizeof(Notification) * MAX_NOTIFICATIONS);
    capacity = MAX_NOTIFICATIONS;
    count = 0;

    loadFromFile(NOTIFICATION_PATH, notifications, sizeof(Notification),
                 MAX_NOTIFICATIONS, &count);
    maxCount = count;
}

// Persists the in-memory notification list to disk.
void saveNotification()
{
    saveToFile(NOTIFICATION_PATH, notifications, sizeof(Notification), count);
}

// 2.7 Show fine statistics by team
// Aggregates paid and unpaid fines by team and prints a summary table.
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

// Displays notifications matching a specific type.
void displayNotifications(int type)
{
    for (int i = 0; i < count; i++)
        if (notifications[i].type == type)
            displaySingleNotification(&notifications[i]);
}

// Displays all notifications for one member ID.
void displayWarning(const char *memberId)
{
    for (int i = 0; i < count; i++)
        if (strcmp(notifications[i].memberId, memberId) == 0)
            displaySingleNotification(&notifications[i]);
}

// Displays notifications for one member and one notification type.
void displayNotificationByMemberID(const char *studentId, int type)
{
    for (int i = 0; i < count; i++)
    {
        Notification *n = &notifications[i];
        if (strcmp(n->memberId, studentId) == 0 && n->type == type)
            displaySingleNotification(n);
    }
}

// Displays every global notification.
void displayGlobalNotification()
{
    for (int i = 0; i < count; i++)
        if (notifications[i].type == GLOBAL_NOTICE)
            displaySingleNotification(&notifications[i]);
}

// Displays the admin notification list with stable IDs.
void displayNotificationList()
{
    for (int i = 0; i < count; i++)
    {
        displayNotificationInAdminMode(&notifications[i]);
        char safeId[7];
        strncpy(safeId, notifications[i].id, 6);
        safeId[6] = '\0';
        printf("Id: %s\n", safeId);
    }
}

// Renders one notification in a human-readable card format.
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

void displayNotificationInAdminMode(Notification *n)
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

    if (n->type == ADMIN_NOTICE || n->type == ADMIN_WARNING)
    {
        printf("Member ID: %s\n", isBlank(n->memberId) ? "None" : n->memberId);
    }

    char deleteTime[20];
    getFormatTime(deleteTime, 20, n->deleteTime);
    printf("Delete time: %s. Send to: %s\n", deleteTime, n->memberId);
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
    memset(&n, 0, sizeof(Notification));

    snprintf(n.id, sizeof(n.id), "%05d", maxCount + 1);
    n.type = type;
    n.deleteTime = deleteTime;
    n.create_time = time(NULL);
    
    strncpy(n.content, message, sizeof(n.content) - 1);
    n.content[sizeof(n.content) - 1] = '\0';
    
    if (receiverID != NULL)
    {
        strncpy(n.memberId, receiverID, sizeof(n.memberId) - 1);
        n.memberId[sizeof(n.memberId) - 1] = '\0';
    }
    
    notifications[count] = n;
    if (isSave)
    {
        count++;
        maxCount++;
        saveNotification();
        return &notifications[count - 1];
    }
    return &notifications[count];
}

Notification *findNotificationById(const char *notificationId)
{
    if (notificationId == NULL) return NULL;
    int len = strlen(notificationId);
    for (int i = 0; i < count; i++)
    {
        if (len < sizeof(notifications[i].id))
        {
            if (strcmp(notificationId, notifications[i].id) == 0)
                return &notifications[i];
        }
        else if (len == sizeof(notifications[i].id))
        {
            if (strncmp(notificationId, notifications[i].id, len) == 0)
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
    if (receiverID != NULL) {
        strncpy(n->memberId, receiverID, 10);
        n->memberId[9] = '\0';
    } else {
        n->memberId[0] = '\0';
    }
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
            {
                for (int j = i; j < count - 1; j++)
                    notifications[j] = notifications[j + 1];
                count--;
                i--;
            }
        }
    saveNotification();
}

void autoDeleteOutDateNotification()
{
    time_t now = time(NULL);
    for (int i = 0; i < count; i++)
        if (now > notifications[i].deleteTime)
        {
            for (int j = i; j < count - 1; j++)
                notifications[j] = notifications[j + 1];
            count--;
            i--;
        }

    saveNotification();
}

Notification *notifyAdmin(const char *content, const char *adminId, int isSave)
{
    Notification *n = createNotification(adminId, ADMIN_NOTICE, content, time(NULL) + BASE_DELETE_TIME, isSave);
    displaySingleNotification(n);
    return n;
}

Notification *warningMember(const char *content, const char *memberId, int isSave)
{
    return createNotification(memberId, ADMIN_WARNING, content, time(NULL) + BASE_DELETE_TIME, isSave);
}

Notification *globalNotification(const char *content) { return createNotification(NULL, GLOBAL_NOTICE, content, time(NULL) + BASE_DELETE_TIME, 1); }

void quickNotification(const char *content)
{
    displaySingleNotification(createNotification(NULL, GLOBAL_NOTICE, content, time(NULL), NOT_SAVE));
}

void freeNotificationList()
{
    if (notifications == NULL)
        return;
    free(notifications);
}

int listViolationsByTimeRange(Violation violations[], int vCount, Violation results[]){
    time_t start, end;
    inputTimeRange(&start, &end, "TIME RANGE");

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
void exportViolationReportToFile(MemberList *members, ViolationList *violations)
{
    if (members == NULL || violations == NULL)
    {
        printf("\033[1;31m[ERROR] Invalid data!\033[0m\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char filename[100];
    strftime(filename, sizeof(filename), "FCODE_ViolationReport_%Y%m%d_%H%M.txt", timeinfo);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("\033[1;31m[ERROR] Cannot create report file!\033[0m\n");
        return;
    }

    char timeStr[50];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(file, "VIOLATION REPORT\n");
    fprintf(file, "Generated at: %s\n\n", timeStr);

    const char *teamNames[] = {"Academic", "Planning", "HR", "Media"};
    double paidByTeam[4] = {0};
    double unpaidByTeam[4] = {0};
    int violationCountByTeam[4] = {0};
    int memberCountByTeam[4] = {0};

    for (int i = 0; i < violations->count; i++)
    {
        Violation *v = &violations->data[i];
        int mIndex = searchMemberByIdInM(members, v->owner->studentID);
        if (mIndex == -1)
            continue;

        int team = members->data[mIndex].team;
        if (team < 0 || team > 3)
            continue;

        violationCountByTeam[team]++;
        if (v->isPaid == 1)
        {
            paidByTeam[team] += v->fine;
        }
        else if (v->isPaid == 0)
        {
            unpaidByTeam[team] += v->fine;
        }
    }

    for (int team = 0; team < 4; team++)
    {
        for (int i = 0; i < members->count; i++)
        {
            int hasViolation = 0;
            if (members->data[i].team != team)
                continue;

            for (int v = 0; v < violations->count; v++)
            {
                if (strcmp(violations->data[v].owner->studentID,
                           members->data[i].studentID) == 0)
                {
                    hasViolation = 1;
                    break;
                }
            }

            if (hasViolation)
                memberCountByTeam[team]++;
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

    for (int i = 0; i < 4; i++)
    {
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

    for (int team = 0; team < 4; team++)
    {
        int membersInTeam = 0;

        for (int i = 0; i < members->count; i++)
        {
            Member *m = &members->data[i];
            if (m->team != team)
                continue;

            double unpaidAmount = 0;
            int unpaidCount = 0;

            for (int v = 0; v < violations->count; v++)
            {
                Violation *viol = &violations->data[v];
                if (strcmp(viol->studentID, m->studentID) == 0 && viol->isPaid == 0)
                {
                    unpaidAmount += viol->fine;
                    unpaidCount++;
                }
            }

            if (unpaidAmount > 0)
            {
                char fineStr[25];
                formatCurrency(unpaidAmount, fineStr, sizeof(fineStr));
                fprintf(file, "| %-10s | %-22s | %-12s | %10d | %-16s |\n",
                        m->studentID, m->fullName, teamNames[team], unpaidCount, fineStr);

                totalOutstanding += unpaidAmount;
                outstandingCount++;
                membersInTeam++;
            }
        }

        if (membersInTeam == 0)
        {
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
                            "Notification type (0=global, 1=notice, 2=warning): ");
        char content[200];
        inputString(content, sizeof(content), "Enter notification content: ");

        printf("Content is: %s. And is blank: %d\n", content, isBlank(content));

        // Content must not be blank
        if (isBlank(content))
        {
            notifyAdmin("Content cannot be blank.", NULL, NOT_SAVE);
            return;
        }

        char memberId[10] = "";
        inputString(memberId, sizeof(memberId), "Enter member id: ");

        int valid = 1;
        if (type == ADMIN_NOTICE &&
            !isBlank(memberId) && !isValidStudentID(memberId))
        {
            notifyAdmin("Invalid member ID for notice.", NULL, NOT_SAVE);
            valid = 0;
        }
        else if (type == ADMIN_WARNING &&
                 !isValidStudentID(memberId))
        {
            notifyAdmin("Invalid member ID for warning.", NULL, NOT_SAVE);
            valid = 0;
        }
        else if (type == GLOBAL_NOTICE &&
                 !isBlank(memberId))
        {
            notifyAdmin("Global notice should not have a member ID.", NULL, NOT_SAVE);
            valid = 0;
        }

        if (valid)
        {
            int confirm = 1;
            inputYesNo(&confirm, "Create this notification? (1: Yes, 0: No): ");
            if (confirm)
            {
                createNotification((isBlank(memberId) ? NULL : memberId),
                                   type, content, time(NULL) + BASE_DELETE_TIME, 1);
                notifyAdmin("Notification created.", NULL, NOT_SAVE);
            }
        }

        inputYesNo(&option, "\nAdd another notification? (1: Yes, 0: No): ");
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
        inputString(id, sizeof(id), "\nEnter notification ID: ");
        Notification *n = findNotificationById(id);

        if (n == NULL)
        {
            notifyAdmin("Notification not found.", NULL, NOT_SAVE);
        }
        else
        {
            Notification updateN;
            inputIntegerInRange(&updateN.type, 0, 2,
                                "Notification type (0=global, 1=notice, 2=warning): ");
            inputString(updateN.content, sizeof(updateN.content), "Enter notification content: ");
            inputString(updateN.memberId, sizeof(updateN.memberId), "Enter member ID: ");

            // Validate once
            int valid = 1;
            if (isBlank(updateN.content))
            {
                notifyAdmin("Content cannot be blank.", NULL, NOT_SAVE);
                valid = 0;
            }
            else if (updateN.type == ADMIN_NOTICE &&
                     !isBlank(updateN.memberId) && !isValidStudentID(updateN.memberId))
            {
                notifyAdmin("Invalid member ID for notice.", NULL, NOT_SAVE);
                valid = 0;
            }
            else if (updateN.type == ADMIN_WARNING &&
                     !isValidStudentID(updateN.memberId))
            {
                notifyAdmin("Invalid member ID for warning.", NULL, NOT_SAVE);
                valid = 0;
            }
            else if (updateN.type == GLOBAL_NOTICE &&
                     !isBlank(updateN.memberId))
            {
                notifyAdmin("Global notice should not have a member ID.", NULL, NOT_SAVE);
                valid = 0;
            }

            if (valid)
            {
                int confirm;
                inputYesNo(&confirm, "Update this notification? (1: Yes, 0: No): ");
                if (confirm)
                {
                    updateNotification(n, NULL, updateN.type, updateN.content, n->deleteTime);
                    if (updateN.type != GLOBAL_NOTICE)
                        strncpy(n->memberId, updateN.memberId, sizeof(n->memberId) - 1);
                    notifyAdmin("Notification updated.", NULL, NOT_SAVE);
                }
            }
        }

        inputYesNo(&option, "\nUpdate another notification? (1: Yes, 0: No): ");
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
        char removeId[7];
        inputString(removeId, sizeof(removeId), "\nEnter notification ID to remove: ");
        Notification *foundNotification = findNotificationById(removeId);
        if (foundNotification != NULL)
        {
            int confirm;
            inputYesNo(&confirm, "Delete this notification? (1: Yes, 0: No): ");
            if (confirm)
            {
                deleteNotification(foundNotification);
                notifyAdmin("Notification deleted.", NULL, NOT_SAVE);
            }
        }
        else
        {
            notifyAdmin("Notification not found.", NULL, NOT_SAVE);
        }

        inputYesNo(&option, "\nDelete another notification? (1: Yes, 0: No): ");
        if (option)
        {
            clearScreen();
        }
    } while (option);
}
