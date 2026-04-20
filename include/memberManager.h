#ifndef MEMBER_MANAGER_H
#define MEMBER_MANAGER_H
#include "../include/member.h"
typedef struct {
    Member members[1000];
    int size;
} MemberManager;

void loadMemberFromFile(MemberManager *m);

void saveMemberToFile(MemberManager *m);

void addMember(MemberManager *m);

void removeMember(MemberManager *m);

#endif