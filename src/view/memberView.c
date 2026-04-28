#include <stdio.h>
#include <stdlib.h>

#include "../../include/utils.h"
#include "../../include/view/memberView.h"

void displayOneMemberInfo(Member member) {
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

void displayInSortByVioCount(Member members[], int mCount, int sortMode) {
    Member sortList[mCount];
    for (int i = 0; i < mCount; i++) {
        sortList[i] = members[i];
    }
    Member temp = {0};
    int minIndex = 0, maxIndex = mCount - 1;

    int start = 0, end = mCount - 1;
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

        temp = sortList[maxIndex];
        sortList[maxIndex] = sortList[end];
        sortList[end] = temp;
        end--;

        if (maxIndex != start) {
            temp = sortList[minIndex];
            sortList[minIndex] = sortList[start];
            sortList[start] = temp;
            start++;
        }
    }

    int i, len;
    if (sortMode == 1) {
        i = 0;
        len = mCount - 1;
    } else {
        i = mCount - 1;
        len = 0;
    }
    printf("\n");
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
