#ifndef MEMBER_VIEW_H
#define MEMBER_VIEW_H

#include "../model.h"

void displayOneMemberInfo(Member member);
void displayMemberList(Member members[], int count);
void displayMemberInSortByVioCount(Member members[], int mCount, int sortMode);
void displayMemberInSort(MemberList* list, int isAdmin);
#endif
