#include "../../include/view/violationView.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../include/consoleInput.h"
#include "../../include/report.h"
#include "../../include/utils.h"
#include "../../include/view/viewUtil.h"
#include "../../include/violation.h"
#include "../../include/member.h"

static short teamFilter = 0;
static short reasonFilter = 0;
static short paidFilter = 0;
static short timeRangeFilter = 0;

static int teamFilterSetting;
static int reasonFilterSetting = REASON_NOT_UNIFORM;
static int paidFilterSetting = ALREADY_PAID;
static time_t beginTimeFilterSetting;
static time_t endTimeFilterSetting;
static const int VIOLATION_PAGE_SIZE = 500;
static int violationRowNumber = 0;

static long order = ASC;

static int enableSortOption;
static char currentSortCommand[3];

// Draws the table header used by all violation list views.
void displayViolationTableHeader()
{
    printf("%s"
        "\n┏━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n%s",
        UI_TABLE_BORDER, UI_RESET);
    
    printf(
        "┃ %s%-8s%s ┃ %s%-8s%s ┃ %s%-16s%s ┃ %s%-16s%s ┃ "
        "%s%-8s%s ┃ %s%-16s%s ┃ %s%-10s%s ┃ %s%-28s%s ┃\n",

        UI_TABLE_HEADER, "ID", UI_RESET,
        UI_TABLE_HEADER, "Student", UI_RESET,
        UI_TABLE_HEADER, "Reason", UI_RESET,
        UI_TABLE_HEADER, "Time", UI_RESET,
        UI_TABLE_HEADER, "Fine", UI_RESET,
        UI_TABLE_HEADER, "Paid", UI_RESET,
        UI_TABLE_HEADER, "Penalty", UI_RESET,
        UI_TABLE_HEADER, "Note", UI_RESET
    );

    printf("%s"
        "┣━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n%s",
        UI_TABLE_BORDER, UI_RESET);
}

// Draws the table footer for violation list views.
void displayViolationTableFooter()
{
    printf(
        "┗━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");

    printf("%s", UI_RESET);
}

// Prints one violation row with formatted fields and alternating row style.
void displayViolationRow(const Violation *v)
{
    char timeField[30];
    getFormatTime(timeField, 30, v->violationTime);

    const char *reasonStr;

    switch (v->reason)
    {
    case REASON_NOT_UNIFORM:
        reasonStr = "Not uniform";
        break;
    case REASON_MEETING_ABSENCE:
        reasonStr = "Meeting absence";
        break;
    case REASON_NO_CLUB_ACTIVITY:
        reasonStr = "No club activity";
        break;
    case REASON_VIOLENCE:
        reasonStr = "Violence";
        break;
    default:
        reasonStr = "Unknown";
        break;
    }

    const char *paidStr = v->isPaid ? ((v->isPaid == ALREADY_PAID) ? "Yes" : "NHTP") : "No";
    const char *penaltyStr = v->penalty ? "Kick" : "Financial";
    const char *rowBg = ((violationRowNumber / 5) % 2 == 0) ? UI_RESET : UI_ROW_ALT;

    printf("%s", rowBg);

    printf(
        "┃ %-8s ┃ %-8s ┃ %-16.16s ┃ %-16.16s ┃ %-8.0f ┃ %-16.16s ┃ %-10.10s ┃ %-28.28s ┃\n",
        v->violationID,
        v->studentID,
        reasonStr,
        timeField,
        v->fine,
        paidStr,
        penaltyStr,
        v->note);

    printf("%s", UI_RESET);

    violationRowNumber++;
}

// Displays the full violation list with simple paging.
void displayViolationList(const Violation violations[], int vCount)
{
    int shown = 0;
    int showMore = 1;

    if (vCount <= 0) {
        uiWarning("No violations to display.\n");
        return;
    }

    violationRowNumber = 0;
    displayViolationTableHeader();
    for (int i = 0; i < vCount; i++)
    {
        displayViolationRow(&violations[i]);

        shown++;
        //max: 500 lines
        if (shown % VIOLATION_PAGE_SIZE == 0 && shown < vCount)
        {
            displayViolationTableFooter();
            inputYesNo(&showMore, "\nShow next violations? (1: Yes, 0: No): ");
            if (!showMore) return;
            displayViolationTableHeader();
        }
    }
    displayViolationTableFooter();
}

// Displays only violations that belong to the requested student ID.
void displayViolationByStudentId(const char *id, const ViolationList *violations)
{
    int shown = 0;
    int showMore = 1;

    violationRowNumber = 0;
    displayViolationTableHeader();
    for (int i = 0; i < violations->count; i++)
    {
        if (strcmp(id, violations->data[i].studentID) == 0)
        {
            displayViolationRow(&violations->data[i]);
            shown++;
            if (shown % VIOLATION_PAGE_SIZE == 0 && i < violations->count - 1)
            {
                displayViolationTableFooter();
                inputYesNo(&showMore, "\nShow next violations? (1: Yes, 0: No): ");
                if (!showMore) return;
                displayViolationTableHeader();
            }
        }
    }
    displayViolationTableFooter();
}

// Marks a selected violation as paid and refreshes the member fine total.
void markFineAsPaidView(ViolationList *violations, MemberList *members){
    if (violations->count <= 0){
        printf ("No violations recorded.\n");
        return;
    }
    if (members->count <= 0){
        printf ("No members in the list.\n");
        return;
    }
    int continueMarkFine = 1;
    while (continueMarkFine){
        char violationID[8];
        printf("\n=== Mark Fine as Paid ===\n");
        inputString(violationID, sizeof(violationID), "Enter Violation ID: ");

        int vIndex = getViolationIndexById(violations, violationID);
        if (vIndex == -1){
            uiError("Violation ID not found.\n");
        }
        else if (violations->data[vIndex].isPaid == ALREADY_PAID){
            printf("This violation is already paid.\n");
        }
        else if (violations->data[vIndex].isPaid == NOT_HAVE_TO_PAY){
            printf("No payment needed.\n");
            continue;
        }
        else {
            int confirm;
            inputYesNo(&confirm, "Mark as paid? (1: Yes, 0: No): ");
            if (confirm){
                violations->data[vIndex].isPaid = ALREADY_PAID;
                updateMemberTotalFine(members, violations, violations->data[vIndex].studentID);
                saveViolations(violations);
                saveMembers(members);
                simpleDisplayViolation(&violations->data[vIndex]);
                uiSuccess("Marked as paid successfully.\n");
            }
            else {
                printf ("Fine not marked as paid.\n");
            }
        }     
        inputYesNo(&continueMarkFine, 
            "\nMark another fine?\n1: Yes\n0: No\n=> Your choice: ");
    }
}

// Shows violations in a time range using the filtered display pipeline.
void displayViolationsByTimeRange(const ViolationList *violations)
{
    Violation results[violations->count];
    if (results == NULL)
        return;

    int resultCount = 0;

    // Assuming listViolationsByTimeRange is refactored to accept (const ViolationList*, Violation*)
    resultCount = listViolationsByTimeRange(violations->data, violations->count, results);

    if (resultCount == 0)
    {
        uiError("No violations found in the specified time range.\n");
        
        return;
    }

    violationRowNumber = 0;
    displayViolationList(results,resultCount);

}

// 1.3 View unpaid fines for a member
// Shows only unpaid fines for the current member.
void viewMyUnpaidFines(const char *myStudentID, const ViolationList *violations)
{
    uiInfo("\nUNPAID FINES\n");
    printf("Student ID: %s\n", myStudentID);

    const char *reasonNames[] = {
        "Not uniform",
        "Meeting absence",
        "No club activity",
        "Violence"};

    printf("\n┏━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┓\n");

    printf("┃ %-12s ┃ %-24s ┃ %-20s ┃ %-16s ┃\n",
           "Violation ID",
           "Reason",
           "Time",
           "Fine");

    printf("┣━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━┫\n");

    double total = 0.0;
    int found = 0;
    for (int i = 0; i < violations->count; i++)
    {
        const Violation *v = &violations->data[i];
        if (strcmp(v->studentID, myStudentID) != 0)
            continue;
        if (v->isPaid == ALREADY_PAID || v->isPaid == NOT_HAVE_TO_PAY)
            continue;

        char timeStr[20];
        getFormatTime(timeStr, sizeof(timeStr), v->violationTime);

        const char *reason = (v->reason >= 0 && v->reason <= 3)
                                 ? reasonNames[v->reason]
                                 : "Unknown";

        char fineStr[30];
        formatCurrency(v->fine, fineStr, sizeof(fineStr));

        printf("┃ %-12s ┃ %-24s ┃ %-20s ┃ %16s ┃\n",
               v->violationID, reason, timeStr, fineStr);

        total += v->fine;
        found++;
    }

    printf("┣━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━┫\n");

    if (found == 0)
    {

        printf("┃ %-78s ┃\n",
               "No unpaid fines found.");
    }
    else
    {

        char totalStr[30];

        formatCurrency(total, totalStr, sizeof(totalStr));

        printf("┃ %-61s  ┃ %-16s ┃\n",
               "Total Unpaid Fines",
               totalStr);
    }

    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┛\n");
}

// Prints the team menu used by the filter flow.
void displayTeamSelected()
{
    printf("\nAvailable Teams:\n");
    printf("0. Academic\n");
    printf("1. Planning\n");
    printf("2. HR\n");
    printf("3. Media\n");
}

// Prints the violation-reason menu used by the filter flow.
void displayReasonSelected()
{
    printf("Reasons:\n");
    printf("%d. Not uniform\n", REASON_NOT_UNIFORM);
    printf("%d. Meeting absence\n", REASON_MEETING_ABSENCE);
    printf("%d. No Club activity\n", REASON_NO_CLUB_ACTIVITY);
    printf("%d. Violence\n", REASON_VIOLENCE);
}

// Resets all filter and sort settings to their default state.
void clearOption()
{
    teamFilter = 0;
    reasonFilter = 0;
    paidFilter = 0;
    timeRangeFilter = 0;
    enableSortOption = 0;
}

// Renders the menu used to change filters and sorting options.
void displayChangeFilterOptionMenu()
{
    printf("%s", UI_TABLE_HEADER);
    printf("\nCHANGE FILTER OPTION MENU");
    printf("%s", UI_RESET);
    printf(
            "\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"
            "\n┃  1. Clear all options (ID-based)             ┃"
            "\n┃  2. Sort options                             ┃"
            "\n┃  3. Change order (ASC, DESC)                 ┃"
            "\n┃  4. Team filter                              ┃"
            "\n┃  5. Reason filter                            ┃"
            "\n┃  6. Time filter                              ┃"
            "\n┃  7. Is paid filter                           ┃"
            "\n┃  8. Exit                                     ┃"
            "\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
}

// Lets the user configure sorting and filtering for the violation list.
void changeFilterOption()
{
    int choice;
    int isContinue = 1;

    do
    {
        clearScreen();
        displayChangeFilterOptionMenu();
        inputIntegerInRange(&choice, 1, 8, "\nEnter your choice: ");
        clearScreen();
        switch (choice)
        {
        case 1:
            clearOption();
            printf("All filters cleared.\n");
        break;
        case 2:
        {
            int option;
            printf("%s", UI_TABLE_HEADER);
            printf("CHANGE SORT OPTION\n");
            printf("\nSort rules\n");
            printf("%s", UI_RESET);
            printf(
                "┏━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"
                "┃ Command ┃ Meaning                             ┃\n"
                "┣━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
                "┃ r       ┃ Sort by reason ASC                  ┃\n"
                "┃ R       ┃ Sort by reason DESC                 ┃\n"
                "┃ p       ┃ Sort by paid ASC                    ┃\n"
                "┃ P       ┃ Sort by paid DESC                   ┃\n"
                "┃ t       ┃ Sort by team ASC                    ┃\n"
                "┃ T       ┃ Sort by team DESC                   ┃\n"
                "┗━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"
            );

            printf("%s", UI_TABLE_HEADER);
            printf("\nExamples of valid commands:\n");
            printf("%s", UI_RESET);
            printf("  rtp   -> reason ASC, team ASC, paid ASC\n");
            printf("  prt   -> paid ASC, reason ASC, team ASC\n");
            printf("  PrT   -> paid DESC, reason ASC, team DESC\n");
            printf("  tR    -> team ASC, reason DESC\n");
            printf("  R     -> reason DESC\n");
            printf("  P     -> paid DESC\n");
            printf("  T     -> team DESC\n");
            printf("  t     -> team ASC\n");

            inputString(currentSortCommand, 4, "Enter sort command: ");
            inputYesNo(&enableSortOption, "Enable sorting? (1: Yes, 0: No): ");
        }
        break;
        case 3:
            printf("Sort order changed to %s.\n", (order) ? "ASC" : "DESC");
            order = !order;
            break;
        case 4:
            displayTeamSelected();
            inputMemberTeam(&teamFilterSetting, "Enter team (0-3): ");
            teamFilter = 1;
            break;
        case 5:
            displayReasonSelected();
            inputIntegerInRange(&reasonFilterSetting, 0, 3, "Enter choice: ");
            reasonFilter = 1;
            break;
        case 6:
            inputTimeRange(&beginTimeFilterSetting, &endTimeFilterSetting, "TIME FILTER");
            timeRangeFilter = 1;
            break;
        case 7:
            inputIntegerInRange(&paidFilterSetting, 0, 1, "Not paid (0), Is paid (1): ");
            printf("\nPaid filter on. Now: %s\n", (paidFilterSetting) ? "Is paid" : "Not paid");
            paidFilter = 1;
            break;
        case 8:
            return;
        default:
            printf("\nInvalid command.");
            break;
        }
        pauseProgram();
    } while (isContinue);
}

// Returns whether compareTime falls inside the supplied range.
int isInTimeRange(time_t compareTime, time_t beginTime, time_t endTime) { return beginTime <= compareTime && compareTime <= endTime; }

// Evaluates the active filter set for one violation row.
int isFiltered(Violation *v, int team)
{
    return (!teamFilter || team == teamFilterSetting) && (!reasonFilter || reasonFilterSetting == v->reason) &&
           (!timeRangeFilter || isInTimeRange(v->violationTime, beginTimeFilterSetting, endTimeFilterSetting)) && (!paidFilter || paidFilterSetting == v->isPaid);
}

// Displays the violation list using the active filter, time, and sort settings.
void flexibleDisplayViolationList(ViolationList* violations, MemberList* members)
{
    if (teamFilter)
    {
        char team[16];
        switch (teamFilterSetting)
        {
        case 0:
            sprintf(team, "Academic");
            break;
        case 1:
            sprintf(team, "Planning");
            break;
        case 2:
            sprintf(team, "HR");
            break;
        case 3:
            sprintf(team, "Media");
            break;
        default:
            sprintf(team, "Unknown");
            break;
        }
        printf("Violation list is displayed by team %s\n", team);
    }

    violationRowNumber = 0;
    

    Violation *tempList[violations->count];
    if (enableSortOption)
    {
        for (int i = 0; i < violations->count; i++)
        {
            tempList[i] = &violations->data[i];
        }
        sortViolation(violations, tempList, currentSortCommand);
        
        int len = 3; // 3 is max factor of violation sort
        if (strlen(currentSortCommand) < 3) len = strlen(currentSortCommand);
        uiTableTitle("ORDER BY: ");
        for (int i = 0; i < len; i++)
        {
        	switch(currentSortCommand[i]){
        		case 'r':
        			printf(" | DESC Role");
        			break;
        		case 'R':
        			printf(" | ASC Role");
        			break;
        		case 'p':
        			printf(" | DESC Paid");
        			break;
        		case 'P':
        			printf(" | ASC Paid");
        			break;
        		case 't':
        			printf(" | DESC Team ");
        			break;
        		case 'T':
        			printf(" | ASC Team");
        			break;
			}
		
		}
		printf(" |");
    }
    else
    {
        for (int i = 0; i < violations->count; i++)
        {
            tempList[i] = &violations->data[i];
        }
    }


	displayViolationTableHeader();
    int shown = 0;
    int showMore = 1;
    for (int i = 0; i < violations->count; i++)
    {
        Violation *v = tempList[i];
        Member *m = getMemberById(v->studentID, members);
        if (m == NULL)
            continue;
        if (isFiltered(v, m->team))
        {
            displayViolationRow(v);
            shown++;
            if (shown % VIOLATION_PAGE_SIZE == 0 && i < violations->count - 1)
            {
                displayViolationTableFooter();
                inputYesNo(&showMore, "\nShow next 500 violations? (1: Yes, 0: No): ");
                if (!showMore) return;
                displayViolationTableHeader();
            }
        }
    }

    displayViolationTableFooter();
}
