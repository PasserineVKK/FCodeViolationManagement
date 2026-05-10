#include "../include/violation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/member.h"
#include "../include/model.h"
#include "../include/report.h"
#include "../include/utils.h"
#include "../include/view/viewUtil.h"
#include "../include/view/violationView.h"
#include "../include/view/memberView.h"
#include "../include/validate.h"

#define IS_READ 1
#define DID_NOT_READ 0

static int maxCount;

void simpleDisplayViolation(const Violation *v)
{
    // Assuming displayViolationList handles a single item if count is 1
    displayViolationList((Violation *)v, 1);
}

void initViolationList(ViolationList *list, int initialCapacity)
{
    list->count = 0;
    list->capacity = initialCapacity;
    list->data = malloc(sizeof(Violation) * list->capacity);
}

int loadViolations(ViolationList *violations, MemberList *members){
    int isLoadSuccess = loadFromFile(VIOLATIONS_FILE, violations->data, sizeof(Violation) - sizeof(Member *),
                                     MAX_VIOLATIONS, &violations->count);

    if (!isLoadSuccess)
    {
        uiWarning("No initial data");
        violations->count = 0;
        return 0;
    }
    else
    {
        // assign pointer to owner
        for (int i = 0; i < violations->count; i++)
        {
            int mIndex = searchMemberByIdInM(members, violations->data[i].studentID);
            // check whether mIndex valid
            if (mIndex >= 0 && mIndex < members->count)
            {
                // owner is pointer
                violations->data[i].owner = (Member *)&(members->data[mIndex]);
            }
            else
            {
                violations->data[i].owner = NULL;
            }
        }
    }

    if (violations->count > 0){
        const char *lastId = violations->data[violations->count - 1].violationID;

        if (strncmp(lastId, "VIO", 3) == 0)
            sscanf(lastId + 3, "%x", &maxCount);
        else
            sscanf(lastId, "%x", &maxCount);
    }
    else{
        maxCount = 0;
    }

    return 1;
}

int saveViolations(ViolationList *violations)
{
    return saveToFile(VIOLATIONS_FILE, violations->data, sizeof(Violation) - sizeof(Member *), violations->count);
}

double calculateFine(int role, int reason)
{
    if (reason == REASON_VIOLENCE)
        return 0;
    if (role == 0)
        return 20000;
    return 50000;
}

void refreshFineAfterRolechange(const char *memberId, int role, ViolationList *violations)
{
    for (int i = 0; i < violations->count; i++)
    {
        Violation *v = &violations->data[i];
        if (strcmp(v->studentID, memberId) == 0 && v->isPaid == NOT_PAY)
        {
            v->fine = calculateFine(role, v->reason);
        }
    }
}

Violation *findViolationById(const char *violationId, ViolationList *violations)
{
    for (int i = 0; i < violations->count; i++)
    {
        if (strcmp(violations->data[i].violationID, violationId) == 0)
        {
            return &violations->data[i];
        }
    }
    return NULL;
}

int getViolationIndexById(const ViolationList *violations, const char *violationId)
{
    for (int i = 0; i < violations->count; i++)
    {
        if (strcmp(violations->data[i].violationID, violationId) == 0)
            return i;
    }
    return -1;
}

void getReason(int *reason)
{
    printf("Choose violate reason: \n"); // Fixed redundant format arg
    printf("%d - Not uniform\n", REASON_NOT_UNIFORM);
    printf("%d - Meeting absence\n", REASON_MEETING_ABSENCE);
    printf("%d - Not join in Club activity\n", REASON_NO_CLUB_ACTIVITY);
    printf("%d - Violence\n", REASON_VIOLENCE);
    inputIntegerInRange(reason, 0, 3, "Please enter: ");
}

int addViolation(ViolationList *violations, const Violation *newV)
{
    simpleDisplayViolation(newV);
    // Check capacity against list->capacity instead of static MAX_VIOLATIONS
    if (violations->count >= violations->capacity)
        return 0;

    // Use pointer copy directly into memory block
    violations->data[violations->count] = *newV;
    violations->count++;
    saveViolations(violations);
    return 1;
}

void updateIsPaidField(const char *violationId, ViolationList *violations, int value)
{
    Violation *v = findViolationById(violationId, violations);
    if (v == NULL) return;
    else if (v->isPaid == NOT_HAVE_TO_PAY)
    {
        uiError("This violation don't have to pay, can't update paid");
        return;
    }
    else v->isPaid = value;
}

int ensureCapacity(ViolationList *list)
{
    if (list == NULL)
        return 0;

    if (list->count >= list->capacity)
    {
        int newCapacity = (list->capacity == 0) ? 4 : list->capacity * 2;
        Violation *anotherAddress = realloc(list->data, newCapacity * sizeof(Violation));

        if (anotherAddress == NULL)
        {
            fprintf(stderr, "NOT ENOUGH MEMORY TO ALLOCATE!!!\n");
            return 0;
        }

        list->data = anotherAddress;
        list->capacity = newCapacity;
    }
    return 1;
}

void createViolationID(int index, char *buffer)
{
    sprintf(buffer, "%06d", index);
}

void removeOneViolation(ViolationList *list, const char *violationId)
{
    Violation *v = findViolationById(violationId, list);
    if (list == NULL || list->data == NULL || v == NULL || list->count <= 0)
    {
        if (v == NULL)
            notifyAdmin("Can not find violation with given id!", NULL, NOT_SAVE);
        return;
    }
    for (int i = 0; i < list->count; i++)
    {
        // if address of data[i] == address of v
        if (&list->data[i] == v)
        {
            printf("Owner address: %p\n", (void *)v->owner);
            // remove from pending list, or change totalFine
            if (v->reason == REASON_VIOLENCE || v->owner->consecutiveAbsences == 3)
            {
                v->owner->isPending = 0;
            }
            else
            {
                v->owner->totalFine = (v->owner->totalFine) - ((v->owner->role == 0) ? 20000 : 50000);
            }

            // if reason is meeting absence -> reduce absence times
            if (v->reason == REASON_MEETING_ABSENCE)
            {
                v->owner->consecutiveAbsences--;
            }

            for (int j = i; j < list->count - 1; j++)
            {
                list->data[j] = list->data[j + 1];
            }
            list->count--;
            break;
        }
    }

    saveViolations(list);
}

// Only for user
void deleteViolation(ViolationList *list)
{
    char violationId[10];
    inputString(violationId, 10, "Enter violation id: ");
    removeOneViolation(list, violationId);
}

void handleSeriousViolation(const Member *m, const Violation *newV)
{
    char content[200];
    if (m == NULL || newV == NULL)
        return;

    const char *studentID = m->studentID;

    if (newV->reason == REASON_MEETING_ABSENCE)
    {
        snprintf(content, sizeof(content),
                 "Member with ID %s. You have been absent from meetings. "
                 "If this violation occurs more than twice, you will be "
                 "kicked out. "
                 "Please contact the administrator if the attendance record "
                 "is incorrect.",
                 studentID);
        warningMember(content, studentID, WILL_SAVE);
    }
    else if (newV->reason == REASON_VIOLENCE)
    {
        snprintf(content, sizeof(content),
                 "Member with ID %s. You have been kicked out for a "
                 "violence-related violation.",
                 studentID);
        warningMember(content, studentID, WILL_SAVE);
    }
}

int checkTotalBOD(MemberList *members)
{
    int bodCount = 0;
    for (int i = 0; i < members->count; i++)
    {
        if (members->data[i].role == 2)
        {
            bodCount++;
        }
    }
    return bodCount;
}

void recordViolationView(ViolationList *violations, MemberList *members, int actorIndex)
{
    if (violations->count > MAX_3_DIGIT_HEX){
        printf ("Note enough space to record violation");
        while (getchar() != '\n');
        return;
    }

    int continueRecord = 1;
    while (continueRecord)
    {
        if (!ensureCapacity(violations))
        {
            printf("Memory allocation failed!\n");
            continue;
        }
        int mIndex;

        char studentID[10];
        int reason;

        time_t violationTime;
        double fine;
        int isPaid;
        int penalty;
        char note[100];
        char violationID[7];

        uiTableTitle("   RECORD VIOLATION");
        inputStudentID(studentID, "Enter Student ID: ");

        mIndex = searchMemberByIdInM(members, studentID);
        
        if (members->data[actorIndex].role == 1 && members->data[mIndex].role == 2){
        	uiError("You are only Leader/Vice. Can not record violations for BOD");
        	return;
		}
        if (mIndex == -1){
            uiError("Error: Student ID not found.\n");
            continue;
        } else { 
            // Pointer directly points to the member in array to avoid shallow copy
            Member *owner = &members->data[mIndex];
            
            if (owner->isPending == 1){
                printf ("This member has already been kicked. "
                    "No more violations can be added. "
                    "Please process this person from the kick list.\n");

            }
            else {
                printf("Reasons:\n");
                printf("%d. Not uniform\n", REASON_NOT_UNIFORM);
                printf("%d. Meeting absence\n", REASON_MEETING_ABSENCE);
                printf("%d. No Club activity\n", REASON_NO_CLUB_ACTIVITY);
                printf("%d. Violence\n", REASON_VIOLENCE);
                inputIntegerInRange(&reason, 0, 3, "Enter reason: ");

                inputString(note, 100, "Enter note (optional): ");

                violationTime = time(NULL);

                int confirm;
                inputYesNo(&confirm, "Confirm to record this violation? (1: Yes, 0: No): ");
                if (!confirm){
                    printf("Violation not recorded.\n");
                }
                else{
                    Violation newV;

                    if (reason == REASON_MEETING_ABSENCE){
                        owner->consecutiveAbsences++;
                    }
                    if (owner->consecutiveAbsences >= 3 || reason == REASON_VIOLENCE){
                        penalty = PENALTY_KICK;
                        fine = 0;
                        isPaid = NOT_HAVE_TO_PAY;
                        owner->isPending = 1;
                    }
                    else{
                        fine = calculateFine(owner->role, reason);
                        isPaid = NOT_PAY;
                        penalty = PENALTY_FINANCIAL;
                    }

                    //Make newID in hexa
                    snprintf(violationID, sizeof(violationID), "VIO%03X", maxCount + 1);

                    strcpy(newV.studentID, studentID);
                    strcpy(newV.violationID, violationID);
                    newV.reason = reason;
                    newV.violationTime = violationTime;
                    strcpy(newV.note, note);
                    newV.fine = fine;
                    newV.isPaid = isPaid;   // 0 = Not yet, 1 = Already
                    newV.penalty = penalty; // 0 = Financial penalty, 1 = Kick
                    newV.owner = owner;

                    if (!addViolation(violations, &newV)) {
                        uiError("Error: Violation list is full.\n");
                        continue;
                    }

                    if (reason == REASON_MEETING_ABSENCE || reason == REASON_VIOLENCE){
                        handleSeriousViolation(owner, &newV);
                    }

                    updateMemberTotalFine(members, violations, studentID);

                    saveViolations(violations);
                    saveMembers(members);

                    uiSuccess("Violation recorded successfully.\n");
                    maxCount++;
                }
            }    
        }

        inputYesNo(&continueRecord, "\nRecord another violation?\n1: Yes\n0: No\n=> Your choice: ");
    }
}

// 2.8 Check and warn if member reach out club condition
void checkAndWarnOutClub(MemberList *members, AccountList *accounts, ViolationList *violations, const char *actorID)
{
    int continueProcess = 1;

    while (continueProcess)
    {
        clearScreen();
        displayWarningList(members, violations);
        displayKickList(members, violations);

        char studentKickedID[10];
        inputString(studentKickedID, 10, "\nEnter student ID from kick list to process (or press Enter to back): ");

        if (strcmp(studentKickedID, "") == 0)
        {
            return;
        }

        // validate studentID
        if (!isValidStudentID(studentKickedID))
        {
            uiError("Please enter a valid student ID.\n");
        }

        int mIndex = searchMemberByIdInM(members, studentKickedID);
        if (mIndex == -1)
        {
            uiError("Member not found.\n");
            printf("Press Enter to continue...");
            while (getchar() != '\n')
                ;
            continue;
        }

        Member *targetMem = &members->data[mIndex];

        if (targetMem->isPending != 1)
        {
            uiError("This member is not in kick list.\n");
            printf("Press Enter to continue...");
            while (getchar() != '\n')
                ;
            continue;
        }

        removeOneMember(members, accounts, violations, studentKickedID, actorID);

        inputYesNo(&continueProcess, "\nDo you want to process another member?\n1. Yes\n0. No\n=> Your choice: ");
    }
}

// Display members in warning list
void displayWarningList(const MemberList *members, const ViolationList *violations)
{

    int firstIndex = -1;
    for (int i = 0; i < members->count; i++)
    {
        if (isMemberInWarningList(&members->data[i], violations))
        {
            firstIndex = i;
        }
    }

    if (firstIndex == -1)
    {
        uiError("No members in warning list.\n");
    }
    else
    {
        uiTableTitle("WARNING LIST");
        printf("\n┏━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┓\n");

        printf("┃ %-12s ┃ %-24s ┃ %-20s ┃\n",
               "Student ID",
               "Full Name",
               "Consecutive Absence");

        printf("┣━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━┫\n");

        int found = 0;
        for (int i = firstIndex; i < members->count; i++)
        {
            if (isMemberInWarningList(&members->data[i], violations))
            {
                printf("┃ %-12s ┃ %-24s ┃ %-20d ┃\n",
                       members->data[i].studentID,
                       members->data[i].fullName,
                       members->data[i].consecutiveAbsences);
            }
        }

        printf("┗━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┛\n");
    }
}

// Display members in kick list
void displayKickList(const MemberList *members, const ViolationList *violations)
{
    uiTableTitle("KICK LIST");
    printf("\n┏━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┓\n");

    printf("┃ %-12s ┃ %-24s ┃ %-21s ┃ %-20s ┃\n",
           "Student ID",
           "Full Name",
           "Consecutive Absence",
           "Reason");

    printf("┣━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━┫\n");

    int found = 0;
    for (int i = 0; i < members->count; i++)
    {
        const Member *mem = &members->data[i];
        if (mem->isPending == 1)
        {

            int hasViolence = hasViolenceViolation(
                mem->studentID,
                violations);

            int hasAbsence =
                mem->consecutiveAbsences >= 3;

            char reason[40] = "";

            if (hasAbsence && hasViolence)
            {
                strcpy(reason, "Absence + Violence");
            }
            else if (hasViolence)
            {
                strcpy(reason, "Violence");
            }
            else
            {
                strcpy(reason, "Absence Threshold");
            }

            printf("┃ %-12s ┃ %-24s ┃ %-21d ┃ %-20s ┃\n",
                   mem->studentID,
                   mem->fullName,
                   mem->consecutiveAbsences,
                   reason);
            found = 1;
        }
    }

    if (!found)
    {
        printf("┃ %-86s ┃\n",
               "No members in kick list.");
    }

    printf("┗━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┛\n");
}

// Check if member has any violence-related violation
int hasViolenceViolation(const char *studentID, const ViolationList *violations)
{
    for (int i = 0; i < violations->count; i++)
    {
        if (strcmp(violations->data[i].studentID, studentID) == 0 &&
            violations->data[i].reason == REASON_VIOLENCE)
        {
            return 1;
        }
    }
    return 0;
}

int isMemberInWarningList(const Member *member, const ViolationList *violations)
{
    if (member->isPending == 1)
        return 0;
    if (member->consecutiveAbsences == 2)
        return 1;
    return 0;
}
