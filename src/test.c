#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "../include/auth.h"
#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/model.h"
#include "../include/report.h"
#include "../include/utils.h"
#include "../include/validate.h"
#include "../include/view/memberView.h"
#include "../include/view/viewUtil.h"
#include "../include/view/violationView.h"
#include "../include/violation.h"
// ====================== MOCK DATA ======================
void initMockMemberData(MemberList* list) {
    list->count = 0;
    
    // ==================== MEMBER 1 ====================
    strcpy(list->data[0].fullName, "Nguyen Van A");
    strcpy(list->data[0].email, "a@gmail.com");
    strcpy(list->data[0].phoneNumber, "0123456789");
    strcpy(list->data[0].studentID, "SE000001");
    list->data[0].team = 2;           // HR
    list->data[0].role = 0;           // Member
    list->data[0].violationCount = 3;
    list->data[0].consecutiveAbsences = 2;
    list->data[0].totalFine = 150000.0;
    list->data[0].isPending = 1;

    // ==================== MEMBER 2 ====================
    strcpy(list->data[1].fullName, "Tran Thi B");
    strcpy(list->data[1].email, "b@gmail.com");
    strcpy(list->data[1].phoneNumber, "0987654321");
    strcpy(list->data[1].studentID, "SE000002");
    list->data[1].team = 0;           // Academic
    list->data[1].role = 1;           // Leader
    list->data[1].violationCount = 1;
    list->data[1].consecutiveAbsences = 5;
    list->data[1].totalFine = 50000.0;
    list->data[1].isPending = 0;

    // ==================== MEMBER 3 ====================
    strcpy(list->data[2].fullName, "Le Van C");
    strcpy(list->data[2].email, "c@gmail.com");
    strcpy(list->data[2].phoneNumber, "0912345678");
    strcpy(list->data[2].studentID, "SE000003");
    list->data[2].team = 2;           // HR
    list->data[2].role = 0;           // Member
    list->data[2].violationCount = 3;
    list->data[2].consecutiveAbsences = 1;
    list->data[2].totalFine = 200000.0;
    list->data[2].isPending = 1;

    // ==================== MEMBER 4 ====================
    strcpy(list->data[3].fullName, "Pham Thi D");
    strcpy(list->data[3].email, "d@gmail.com");
    strcpy(list->data[3].phoneNumber, "0932123456");
    strcpy(list->data[3].studentID, "SE000004");
    list->data[3].team = 1;           // Planning
    list->data[3].role = 2;           // BOD
    list->data[3].violationCount = 0;
    list->data[3].consecutiveAbsences = 0;
    list->data[3].totalFine = 0.0;
    list->data[3].isPending = 0;

    // ==================== MEMBER 5  ====================
    strcpy(list->data[4].fullName, "Hoang Minh E");
    strcpy(list->data[4].email, "e@gmail.com");
    strcpy(list->data[4].phoneNumber, "0978123456");
    strcpy(list->data[4].studentID, "SE000005");
    list->data[4].team = 3;           // Media
    list->data[4].role = 0;           // Member
    list->data[4].violationCount = 5;
    list->data[4].consecutiveAbsences = 4;
    list->data[4].totalFine = 450000.0;
    list->data[4].isPending = 1;

    // ==================== MEMBER 6  ====================
    strcpy(list->data[5].fullName, "Phan Thi F");
    strcpy(list->data[5].email, "f@gmail.com");
    strcpy(list->data[5].phoneNumber, "0909123456");
    strcpy(list->data[5].studentID, "SE000006");
    list->data[5].team = 0;           // Academic
    list->data[5].role = 1;           // Leader
    list->data[5].violationCount = 2;
    list->data[5].consecutiveAbsences = 3;
    list->data[5].totalFine = 80000.0;
    list->data[5].isPending = 0;

    // ==================== MEMBER 7 ====================
    strcpy(list->data[6].fullName, "Dang Van G");
    strcpy(list->data[6].email, "g@gmail.com");
    strcpy(list->data[6].phoneNumber, "0911987654");
    strcpy(list->data[6].studentID, "SE000007");
    list->data[6].team = 2;           // HR
    list->data[6].role = 0;           // Member
    list->data[6].violationCount = 4;
    list->data[6].consecutiveAbsences = 0;
    list->data[6].totalFine = 120000.0;
    list->data[6].isPending = 1;

    // ==================== MEMBER 8 ====================
    strcpy(list->data[7].fullName, "Vu Thi H");
    strcpy(list->data[7].email, "h@gmail.com");
    strcpy(list->data[7].phoneNumber, "0933555666");
    strcpy(list->data[7].studentID, "SE000008");
    list->data[7].team = 1;           // Planning
    list->data[7].role = 2;           // BOD
    list->data[7].violationCount = 1;
    list->data[7].consecutiveAbsences = 2;
    list->data[7].totalFine = 30000.0;
    list->data[7].isPending = 0;

    // ==================== MEMBER 9  ====================
    strcpy(list->data[8].fullName, "Bui Minh I");
    strcpy(list->data[8].email, "i@gmail.com");
    strcpy(list->data[8].phoneNumber, "0988776655");
    strcpy(list->data[8].studentID, "SE000009");
    list->data[8].team = 3;           // Media
    list->data[8].role = 0;           // Member
    list->data[8].violationCount = 3;
    list->data[8].consecutiveAbsences = 6;
    list->data[8].totalFine = 250000.0;
    list->data[8].isPending = 1;

    list->count = 9;
}
// ====================== HELPER PRINT ======================
void printMember(Member* m) {
    printf("%-15s | Team: %-8s | Role: %-20s | Viol: %d | Fine: %8.0f | Pending: %d\n",
           m->fullName,
           translateTeam(m->team),
           translateRole(m->role),
           m->violationCount,
           m->totalFine,
           m->isPending);
}

void printMemberList(Member* ptrList[], int count) {
    printf("=== SORTED RESULT (%d members) ===\n", count);
    for (int i = 0; i < count; i++) {
        printMember(ptrList[i]);
    }
    printf("==================================\n\n");
}

// ====================== MAIN TEST ======================
int main() {
    MemberList list;
    Member* sortPointerList[100];  

    printf("=== UNIT TEST - MULTI CRITERIA SORT ===\n\n");

    initMockMemberData(&list);

    
    for (int i = 0; i < list.count; i++) {
        sortPointerList[i] = &list.data[i];
    }

    // ==================== TEST CASES ====================

    printf("1. Test sort violationCount DESC (v)\n");
    sortMember(&list, sortPointerList, "v");
    printMemberList(sortPointerList, list.count);

    printf("2. Test sort totalFine DESC (f)\n");
    sortMember(&list, sortPointerList, "f");
    printMemberList(sortPointerList, list.count);

    printf("3. Test sort violation DESC -> fine DESC (vf)\n");
    sortMember(&list, sortPointerList, "vf");
    printMemberList(sortPointerList, list.count);

    printf("4. Test sort team ASC -> violation DESC (Tv)\n");
    sortMember(&list, sortPointerList, "Tv");
    printMemberList(sortPointerList, list.count);

    printf("5. Test sort role DESC -> team ASC -> fine DESC (Rtf)\n");
    sortMember(&list, sortPointerList, "Rtf");
    printMemberList(sortPointerList, list.count);

    printf("6. Test sortuppercase (Team ASC)\n");
    sortMember(&list, sortPointerList, "T");
    printMemberList(sortPointerList, list.count);

    printf("7. Test sort ( not crash)\n");
    sortMember(&list, sortPointerList, "");
    printMemberList(sortPointerList, list.count);

    printf("=== FINISH TEST ===\n");
    return 0;
}
