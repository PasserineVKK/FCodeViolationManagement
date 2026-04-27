#ifndef REPORT_H
#define REPORT_H

#include "model.h"

void displayNotification(Notification *list, int count);

void createNotification(Notification **list, int *count, int *capacity,
                        const char *receiverID, int type, const char *message, time_t deleteTime);
void updateNotification(Notification *n,
                        const char *receiverID, int type, const char *message, time_t deleteTime);
void deleteNotification(Notification *n, Notification **list, int *count);

Notification *findNotificationById(const char *notificationId, Notification *list, int count);

#endif