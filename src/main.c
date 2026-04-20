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
	
	int mCount = 0, vCount = 0, aCount = 0;

	Member members[MAX_MEMBERS];
	Violation violations[MAX_VIOLATIONS];
	Account accounts[MAX_ACCOUNTS];

	mCount = loadMembers(members, &mCount);
	vCount = loadViolations(violations, &vCount);
	aCount = loadAccounts(accounts, &aCount);
	
	return 0;
	
}

