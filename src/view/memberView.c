#include <stdio.h>
#include <stdlib.h>

#include "../../include/utils.h"
#include "../../include/view/memberView.h"
#include "../../include/view/viewUtil.h"
#include "../../include/model.h"
#include "../../include/consoleInput.h"

void displayOneMemberInfo(Member member) {
    uiTableTitle("MEMBER PROFILE");
    printf(
        "┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━┓\n");

    printf("┃ %-10s ┃ %-24s ┃ %-28s ┃ %-12s ┃ %-12s ┃ %-18s ┃ %-8s ┃\n",
           "Student ID", "Name", "Email", "Phone", "Team", "Role", "Status");

    printf(
        "┣━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━┫\n");

    printf("┃ %-10s ┃ %-24s ┃ %-28s ┃ %-12s ┃ %-12s ┃ %-18s ┃ %-8s ┃\n",
           member.studentID, member.fullName, member.email, member.phoneNumber,
           translateTeam(member.team), translateRole(member.role),
           member.isPending ? "Pending" : "Active");

    printf(
        "┗━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━┛\n");
}

void displayMemberList(Member members[], int count) {
    printf("\n");
    uiTableTitle("CLUB MEMBER LIST");
    printf(
        "┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━┓\n");

    printf("┃ %-10s ┃ %-22s ┃ %-12s ┃ %-18s ┃\n", "Student ID", "Full Name",
           "Team", "Role");

    printf(
        "┣━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━┫\n");

    for (int i = 0; i < count; i++) {
        printf("┃ %-10s ┃ %-22s ┃ %-12s ┃ %-18s ┃\n", members[i].studentID,
               members[i].fullName, translateTeam(members[i].team),
               translateRole(members[i].role));
    }

    printf(
        "┗━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━┛\n");
    printf("Total members: %d\n\n", count);
}



void displayMemberInSortByVioCount(Member members[], int mCount, int sortMode) {
	// 1. Prepare a copy list before sorting
    Member sortList[mCount];
    for (int i = 0; i < mCount; i++) {
        sortList[i] = members[i];
    }
    Member temp = {0};
    int minIndex = 0, maxIndex = mCount - 1;

    int start = 0, end = mCount - 1;
    // 2. Double Selection Sort
    while (start < end) {
        minIndex = start;
        maxIndex = end;
        for (int j = start; j <= end; j++) {
            if (sortList[j].violationCount <=
                sortList[minIndex].violationCount) {
                minIndex = j;
            }

            if (sortList[j].violationCount >=
                sortList[maxIndex].violationCount) {
                maxIndex = j;
            }
        }

		// swap max value to the end
        temp = sortList[maxIndex];
        sortList[maxIndex] = sortList[end];
        sortList[end] = temp;
        
        // check edge case: if the minIndex is in end, after swap, the minIndex must be the maxIndex
		if (minIndex == end){
			minIndex = maxIndex;
		}
      
        temp = sortList[minIndex];
        sortList[minIndex] = sortList[start];
        sortList[start] = temp;
        
        start++;
        end--;
        
    }
	
	// 3. Decide to show in ASC or DESC by sortMode: 1 is ASC, -1 is DESC
    int i, len;
    if (sortMode == 1) {
        i = 0;
        len = mCount - 1;
    } else {
        i = mCount - 1;
        len = 0;
    }
    printf("\n");
    uiTableTitle("MEMBERS SORTED BY VIOLATION COUNT");
    printf(
        "┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃ %-10s ┃ %-22s ┃ %-10s ┃ %-18s ┃ %-18s ┃\n", "Student ID",
           "Full Name", "Team", "Role", "Violations");
    printf(
        "┣━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━┫\n");
    for (i; i != len + sortMode; i = i + sortMode) {
        printf("┃ %-10s ┃ %-22s ┃ %-10s ┃ %-18s ┃ %-18d ┃\n",
               sortList[i].studentID, sortList[i].fullName,
               translateTeam(sortList[i].team), translateRole(sortList[i].role),
               sortList[i].violationCount);
    }
    printf(
        "┗━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━┛\n");
 }

void displayMemberInSort(MemberList* list, int isAdmin){
	int count = list->count;
	char compareCommand[20];
	
	printf("Uppercase = ASC, Lowercase = DESC.\n");
	printf("[T]eam - [R]ole - [N]ame %s", (isAdmin) ? "- Total[Fine] - [V]iolationCount" : "");
	printf("\ne.g: Tn - Team ascending, Name descending.\n");
	printf("Press Enter if sort is not needed\n");
    inputString(compareCommand, sizeof(compareCommand), "Enter your command: ");
    Member* sortPointerList[count];
    sortMember(list, sortPointerList, compareCommand);
    
	uiTableTitle("CLUB MEMBER SORTED LIST");
	
	    printf(
        "┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━%s┓\n", (isAdmin)?"┳━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━":"");
    printf("┃ %-10s ┃ %-22s ┃ %-10s ┃ %-18s ┃ ", "Student ID",
           "Full Name", "Team", "Role");
           if (isAdmin) printf("%-19s ┃ %-19s ┃", "Violations", "TotalFine");
    printf(
        "\n┣━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━%s┫", (isAdmin)?"╋━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━":"");
    for (int i = 0; i < count; i++) {
        printf("\n┃ %-10s ┃ %-22s ┃ %-10s ┃ %-18s ┃",
               sortPointerList[i]->studentID, sortPointerList[i]->fullName, 
			   translateTeam(sortPointerList[i]->team), translateRole(sortPointerList[i]->role));
			if (isAdmin){
				char formattedFine[20];
				formatCurrency(sortPointerList[i]->totalFine, formattedFine, sizeof(formattedFine));
				printf(" %-19d ┃ %-19s ┃", sortPointerList[i]->violationCount,formattedFine);
			} 
               
    }
    printf(
        "\n┗━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━%s┛\n", (isAdmin)?"┻━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━":"");
 
	
}
