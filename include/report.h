#ifndef REPORT_H
#define REPORT_H

#include "model.h"

// @attention 1 month. After this time, notification will automatic delete.
#define BASE_DELETE_TIME 2592000

// @warning Must call when use notification struc.
// Call free if not using any more
void initNotificationList(int size);

void displayNotifications();

void displaySingleNotification(Notification* n);

Notification* createNotification(const char* receiverID, int type,
                                 const char* message, time_t deleteTime);

void updateNotification(Notification* n, const char* receiverID, int type,
                        const char* message, time_t deleteTime);

void deleteNotification(Notification* n);

Notification* findNotificationById(const char* notificationId);

// 2.7 Show fine statistics by team
void showFineStatsByTeam(Member members[], int mCount, Violation violations[],
                         int vCount);

void notifyAdmin(const char* content, const char* adminId);
void warningMember(const char* content, const char* memberId);
void globalNotification(const char* content);

// @warning Must call after use!
void freeNotificationList();

#endif