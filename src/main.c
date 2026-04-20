#include <stdio.h>
#include <stdlib.h>
#include "../include/utils.h"
#include "../include/member.h"
#include "../include/auth.h"
#include "../include/violation.h"
#include "../include/validate.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	printf("Test module");
	printf("%d", isPosInteger("123"));
	
	Member members[MAX_MEMBERS];
	int mCount = 0;
	loadMembers(members, &mCount);
	addMember(members, &mCount);
	removeMember(members, &mCount);
	
	return 0;
	
}

