#ifndef MEMBER_VIEW_H
#define MEMBER_VIEW_H

#include "../model.h"

void displayOneMemberInfo(Member member);
void displayMemberList(Member members[], int count);
void displayInSortByVioCount(Member members[], int mCount, int sortMode);

// Features 2.7: Show member list
void showTotalFineByRole(Member members[], int count);

#endif