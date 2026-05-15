#ifndef REPORT_H
#define REPORT_H

#include "model.h"

// Notifications: default auto-delete time (1 month)
#define BASE_DELETE_TIME 2592000
#define NOTIFICATION_PATH "data/notification.dat"

// Notification list lifecycle
// Call initNotificationList() before use and freeNotificationList() after.
void initNotificationList();
void freeNotificationList();

// Notification CRUD and helpers
Notification *createNotification(const char *receiverID, int type,
                                 const char *message, time_t deleteTime,
                                 int isSave);
void updateNotification(Notification *n, const char *receiverID, int type,
                        const char *message, time_t deleteTime);
void deleteNotification(Notification *n);
void deleteNotificationByMemberId(const char *memberId);
Notification *findNotificationById(const char *notificationId);
void autoDeleteOutDateNotification();

// Convenience creators
Notification *notifyAdmin(const char *content, const char *adminId, int isSave);
Notification *warningMember(const char *content, const char *memberId, int isSave);
Notification *globalNotification(const char *content);

// Display helpers
void displayNotificationList();
void displaySingleNotification(Notification *n);
void displayNotificationByMemberID(const char *memberId, int type);
void displayGlobalNotification();
void displayNotificationInAdminMode(Notification *n);
void displayWarning(const char *memberId);
void displayNotifications(int type);

// View menu
void createNotificationView(MemberList* members);
void updateNotificationView();
void deleteNotificationView();
void displayNotificationManagerMenu();

// Reports and stats
// 2.7 Show fine statistics by team
void showFineStatsByTeam(MemberList *member, ViolationList *violations);
// Feature 2.11: Lists violations by time.
int listViolationsByTimeRange(Violation violations[], int vCount, Violation results[]);

// 2.10 - Advanced feature: Export violation report to .txt file
void exportViolationReportToFile(MemberList *members, ViolationList *violations);

#endif