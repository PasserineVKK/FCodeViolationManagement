#include <stdio.h>
#include <string.h>
#include "member.h"
#include "violation.h"
#include "fileio.h"

//file
int loadMembers(Member members[], int *count) {
    return loadFromFile(MEMBERS_FILE, members, sizeof(Member), MAX_MEMBERS, count);
}

int saveMembers(Member members[], int count) {
    return saveToFile(MEMBERS_FILE, members, sizeof(Member), count);
}

//search
int searchMemberById(Member members[], int count, const char *id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(members[i].studentID, id) == 0) {
            return i;
        }
    }
    return -1;
}

//update: update tổng tiền nợ cho thành viên
int updateMemberTotalFine(const char *id) {
    Member members[MAX_MEMBERS];
    int mCount = 0;
    if(loadMembers(members, &mCount) == -1 && mCount == 0) {
        printf("Error loading members data.\n");
        return 0;
    }
    int mIndex = searchMemberById(members, mCount, id);
    if(mIndex == -1) {
        printf("Member with ID %s not found.\n", id);
        return 0;
    }
    //tải vi pham của thành viên
    Violation violations[MAX_VIOLATIONS];
    int vCount = 0;
    loadViolations(violations, &vCount);
    //tính tổng tiền nợ
    //chỉ tính những vi phạm chưa được thanh toán và không đang chờ xử lý
    double totalFine = 0.0;
    for(int i = 0; i < vCount; i++) {
        if(strcmp(violations[i].studentID, id) == 0 
        && violations[i].isPaid == 0
        && violations[i].isPending == 0) {
            totalFine += violations[i].fine;
        }
    }
    members[mIndex].totalFine = totalFine;
    saveMembers(members, mCount);
    return 1;
}