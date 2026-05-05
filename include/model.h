#ifndef MODEL_H
#define MODEL_H
#define MAX 1000
#include <time.h>

typedef struct {
    char studentID[10];  // SE000000\0
    char password[30];
    int role;       // 0 = Member, 1 = BOD
    int isLocked;   // 1 = This account locked after 3 failed trials
    int failCount;  // consecutive failed trials
} Account;

typedef struct {
    char fullName[100];
    char email[100];
    char phoneNumber[11];
    char studentID[10];  // SE000000\0
    int team;            // 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
    int role;            // 0 = Member, 1 = Leader/Vice, 2 = BOD
    int violationCount;
    int consecutiveAbsences;
    double totalFine;
    int isPending;  // 0 = not pending, 1 = pending
} Member;


typedef struct {
    char violationID[50];
    char studentID[10];  // SE000000\0
    int reason;  // 0 = Not uniform, 1 = Meeting absence, 2 = Not join in Club
                 // activity, 3 = Violence
    time_t violationTime;
    double fine;
    int isPaid;   // 0 = Not yet, 1 = Already
    int penalty;  // 0 = Financial penalty, 1 = Kick
    char note[100];
  //  int isPending;  // 0 = Not pending, 1 = Pending
    Member* owner;
} Violation;

typedef struct {
	Member data[MAX];
	int count;
} MemberList;

typedef struct {
	Account data[MAX];
	int count;
} AccountList;

typedef struct {
	Violation* data;
	int count;
	int capacity;
} ViolationList;


// Notification

#define MAX_MESSAGE_LENGTH 200

#define NOTI_UNREAD 0
#define NOTI_READ 1

#define GLOBAL_NOTICE 0
#define ADMIN_NOTICE 1
#define ADMIN_WARNING 2

#define WILL_SAVE 1
#define NOT_SAVE 0
typedef struct {
    char id[6];
    int type;
    char memberId[10];  // Only for Admin waring;
    char content[MAX_MESSAGE_LENGTH];
    time_t create_time;
    time_t deleteTime;
} Notification;

#endif
