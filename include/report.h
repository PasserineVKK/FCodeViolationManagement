#ifndef REPORT_H
#define REPORT_H

#include "model.h"

// @attention 1 month. After this time, notification will automatic delete.
#define BASE_DELETE_TIME 2592000

// @attention base notification path
#define NOTIFICATION_PATH "data/notification.dat"

// @warning Must call when use notification struc.
// Call free if not using any more
void initNotificationList();

// @warning Must call after use!
void freeNotificationList();

void displayNotificationList();

void displaySingleNotification(Notification *n);

void displayNotificationByMemberID(const char *memberId, int type);

void displayGlobalNotification();

Notification *createNotification(const char *receiverID, int type,
                                 const char *message, time_t deleteTime,
                                 int isSave);

void updateNotification(Notification *n, const char *receiverID, int type,
                        const char *message, time_t deleteTime);

void deleteNotification(Notification *n);
void deleteNotificationByMemberId(const char *memberId);
void autoDeleteOutDateNotification();

Notification *findNotificationById(const char *notificationId);

// 2.7 Show fine statistics by team
void showFineStatsByTeam(MemberList *member, ViolationList *violations);

// Feature 2.11 Show violation by time range
int listViolationsByTimeRange(Violation violations[], int vCount,
                              Violation results[]);

Notification *notifyAdmin(const char *content, const char *adminId, int isSave);
Notification *warningMember(const char *content, const char *memberId,
                            int isSave);
Notification *globalNotification(const char *content);

// View menu
void createNotificationView();
void updateNotificationView();
void deleteNotificationView();
void displayNotificationManagerMenu();

void displaySingleNotification(Notification *n);
void displayNotificationList();
void displayGlobalNotification();
void displayNotificationByMemberID(const char *studentId, int type);
void displayWarning(const char *memberId);
void displayNotifications(int type);

// 2.10 - Advanced feature: Export violation report to .txt file
// Includes timestamp, summary by team, and members with outstanding fines
void exportViolationReportToFile(MemberList *members, ViolationList *violations);

#endif