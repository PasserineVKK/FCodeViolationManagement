#include <stdio.h>
#include <stdlib.h>
#include "../include/view/viewUtil.h"
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
	Violation *violations = NULL;
    int vCount = 0, vCapacity = 0;
	Account accounts[MAX_ACCOUNTS];

	mCount = loadMembers(members, &mCount);
	loadViolations(&violations, &vCount, &vCapacity);
	aCount = loadAccounts(accounts, &aCount);
    
    // ... logic ...

    if (violations != NULL) {
        freeViolations(&violations, &vCount, &vCapacity);
    }
	
	return 0;
	
}
