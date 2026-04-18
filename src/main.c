#include <stdio.h>
#include <stdlib.h>
#include "../include/member.h"
#include "../include/auth.h"
#include "../include/violation.h"
#include "../include/validate.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	printf("Test module");
	int i = isPosInteger ("03");
	printf ("%d",i );
	printf("Test module");
	return 0;
}
