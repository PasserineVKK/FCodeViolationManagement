#include <stdio.h>
#include <string.h>
#include "../include/report.h"
#include "../include/member.h"
#include "../include/violation.h"
#include "../include/utils.h"

//2.7 Show fine statistics by team
void showFineStatsByTeam(Member members[], int mCount, Violation violations[], int vCount) {
	const char *teamNames[] = {"Academic", "Planning", "HR", "Media"};

    double paidByTeam[4]   = {0};
    double unpaidByTeam[4] = {0};

    for (int i = 0; i < vCount; i++) {
        Violation *v = &violations[i];
        if (v->isPending) continue; //Not count pending violation

        int mIdx = searchMemberByIdInM(members, mCount, v->studentID);
        if (mIdx == -1) continue; 

        int team = members[mIdx].team;
        if (team < 0 || team > 3) continue;

        if (v->isPaid) {
            paidByTeam[team] += v->fine;
        } else {
            unpaidByTeam[team] += v->fine;
        }
    }

    printf("\n===== STATISTICS FINE BY TEAM =====\n");
    printf("%-12s %18s %18s %18s\n", "Team", "Paid", "Unpaid", "Total");
    printf("--------------------------------------------------------------\n");

    double grandPaid = 0, grandUnpaid = 0; 

    for (int i = 0; i < 4; i++) {
        char paidStr[30], unpaidStr[30], totalStr[30];
        formatCurrency(paidByTeam[i], paidStr, sizeof(paidStr));
        formatCurrency(unpaidByTeam[i], unpaidStr, sizeof(unpaidStr));
        formatCurrency(paidByTeam[i] + unpaidByTeam[i], totalStr, sizeof(totalStr));

        printf("%-12s %18s %18s %18s\n",
               teamNames[i], paidStr, unpaidStr, totalStr);

        grandPaid += paidByTeam[i];
        grandUnpaid += unpaidByTeam[i];
    }

    printf("--------------------------------------------------------------\n");
    char gPaid[30], gUnpaid[30], gTotal[30];
    formatCurrency(grandPaid, gPaid, sizeof(gPaid));
    formatCurrency(grandUnpaid, gUnpaid, sizeof(gUnpaid));
    formatCurrency(grandPaid + grandUnpaid, gTotal, sizeof(gTotal));
    printf("%-12s %18s %18s %18s\n", 
	"TOTAL", gPaid, gUnpaid, gTotal);
    printf("\n");
}