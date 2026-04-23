#ifndef VIOLATION_H
#define VIOLATION_H

#include <time.h>
#include <stdlib.h>

// ===== Lý do vi phạm =====
#define REASON_NOT_UNIFORM 0
#define REASON_MEETING_ABSENCE 1
#define REASON_NO_CLUB_ACTIVITY 2
#define REASON_VIOLENCE 3

// ===== Trạng thái pending =====
#define PENDING 1
#define NOT_PENDING 0

// ===== Trạng thái thu tiền =====
#define ALREADY_PAID 1
#define NOT_PAY 0

// ===== Hình phạt =====
#define PENALTY_FINANCIAL 0
#define PENALTY_KICK 1

// ===== File lưu trữ =====
#define VIOLATIONS_FILE "data/violations.dat"

typedef struct
{
	char violationID[50];
	char studentID[10]; // SE000000\0
	int reason;
	time_t violationTime;
	double fine;
	int isPaid;
	int pelnaty;
	char note[100];
	int isPending;
} Violation;

// ===== Prototype cho mảng động =====

// Load dữ liệu từ file vào mảng động
int loadViolations(Violation **violations, int *count, int *capacity);

// Lưu dữ liệu từ mảng động xuống file
int saveViolations(Violation *violations, int count);

// Kiểm tra điều kiện out CLB
int checkOutCondition(const Violation *violations, int count, const char *id);

// Tính mức phạt theo role và reason
double calculateFine(int role, int reason);

// Refresh lại mức phạt sau khi đổi role
void refreshFineAfterRolechange(char *memberId, int role, Violation *violations, int count);

// Tìm vi phạm theo ID
Violation *findViolationById(const char *violationId, Violation *violations, int count);

// Cập nhật trạng thái isPaid
void updateIsPaidField(char *violationId, Violation *violations, int count, int value);

// Tạo vi phạm mới (thêm vào mảng động)
void createNewViolation(Violation **violations, int *count, int *capacity, Member *m);

// Giải phóng bộ nhớ
void freeViolations(Violation **violations, int *count, int *capacity);

#endif // VIOLATION_H
