#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "report.h"

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
    if (n == NULL) return;
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
