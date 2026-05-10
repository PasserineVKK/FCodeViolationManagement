#include "../include/member.h"
#include <stdio.h>
#include <string.h>

#include "../include/auth.h"
#include "../include/consoleInput.h"
#include "../include/fileio.h"
#include "../include/report.h"
#include "../include/utils.h"
#include "../include/validate.h"
#include "../include/view/memberView.h"
#include "../include/view/viewUtil.h"
#include "../include/violation.h"

// file
int loadMembers(MemberList* members) {
    // Assuming MAX_MEMBERS is defined and corresponds to MAX in your struct
    return loadFromFile(MEMBERS_FILE, members->data, sizeof(Member), MAX,
                        &(members->count));
}

int saveMembers(const MemberList* members) {
    return saveToFile(MEMBERS_FILE, members->data, sizeof(Member), members->count);
}

// search
int searchMemberByIdInM(const MemberList *members, const char* id) {
    for (int i = 0; i < members->count; i++) {
        if (strcmp(members->data[i].studentID, id) == 0) {
            return i;
        }
    }
    return -1;
}

int countUnpaidViolations(const char* id, const ViolationList* violations) {
    int unpaidCount = 0;
    for (int i = 0; i < violations->count; i++) {
        // Use pointer to access directly and avoid copying
        Violation* v = &violations->data[i];
        if (strcmp(v->studentID, id) == 0 &&
            v->isPaid == NOT_PAY && v->owner->isPending == NOT_PENDING) {
            unpaidCount++;
        }
    }
    return unpaidCount;
}

// updateTotalFine
int updateMemberTotalFine(MemberList* members, const ViolationList* violations, const char* id) {
    int memberIndex = searchMemberByIdInM(members, id);
    if (memberIndex == -1) {
        printf("Member not found!\n");
        return 0;
    }
    
    double totalFine = 0;
    for (int i = 0; i < violations->count; i++) {
        Violation* v = &violations->data[i];
        if (strcmp(v->studentID, id) == 0 &&
            v->isPaid == NOT_PAY && v->owner->isPending == NOT_PENDING) {
            totalFine += v->fine;
        }
    }
    
    // Use pointer to update directly
    Member* targetMem = &members->data[memberIndex];
    targetMem->totalFine = totalFine;
    return 1;
}

int updateConsecutiveAbsences(MemberList* members, const char* id) {
    int index = searchMemberByIdInM(members, id);

    if (index != -1) {
        // Use pointer to update directly
        Member* targetMem = &members->data[index];
        targetMem->consecutiveAbsences++;
        saveMembers(members);
        //-> function trigger warning if absence = 2 may be insert here
        return 1;
    } else {
        return 0;
    }
}

// ===== Feature 2.1: ADD MEMBER =====
void addMember(MemberList* members, AccountList* accounts, const char *actorID) {
    if (members->count >= MAX) {
        printf("Member list is full!\n");
        return;
    }

    char fullName[100];
    char email[100];
    char phoneNumber[11];
    char studentID[10];  // SE000000\0
    int team;            // 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
    int role;
    int actorIndex = searchMemberByIdInM(members, actorID);

    if (actorIndex == -1) {
        printf("Actor not found!\n");
        return;
    }
    int actorRole = members->data[actorIndex].role;

    int continueAdd = 1;
    // Use loop to let user add multiple member until they choose to stop
    while (continueAdd) {
        // Input student ID
        inputStudentID(studentID, "\nEnter student ID: ");
        if (searchMemberByIdInM(members, studentID) == -1) {
            // Input member name
            inputMemberName(fullName, "\nEnter full name: ");

            // Input member email
            inputMemberEmail(email, "\nEnter email: ");

            // Input member phone number
            inputMemberPhone(phoneNumber, "\nEnter phone number: ");

            // Input team info
            printf("\nAvailable Teams:\n");
            printf("0. Academic\n");
            printf("1. Planning\n");
            printf("2. HR\n");
            printf("3. Media\n");
            inputMemberTeam(&team, "Enter team (0-3): ");

            // Input role info
            printf("\nAvailable Roles:\n");
            printf("0. Member\n");
            printf("1. Leader/Vice\n");
            printf("2. Board of Directors\n");
            inputMemberRole(&role, "Enter role (0-2): ");

            if (role == 2 && actorRole < 2) {
                printf("You are not granted permission to create BOD accounts.\n");
                continue;
            }

            // Confirm to add member
            int confirm;

            inputYesNo(&confirm,
                       "\nAdd this member?\n1: Yes\n0: No\n=> Your choice: ");

            if (confirm == 1) {
                // Use pointers to assign data directly to the memory block, avoiding shallow copy of structs
                Member* newMem = &members->data[members->count];
                strcpy(newMem->fullName, fullName);
                strcpy(newMem->studentID, studentID);
                strcpy(newMem->email, email);
                strcpy(newMem->phoneNumber, phoneNumber);
                newMem->team = team;
                newMem->role = role;
                newMem->violationCount = 0;
                newMem->consecutiveAbsences = 0;
                newMem->totalFine = 0;
                newMem->isPending = NOT_PENDING;

                // Create account for this member with default password "123456" (using ID as default based on logic)
                Account* newAcc = &accounts->data[accounts->count];
                strcpy(newAcc->studentID, studentID);
                strcpy(newAcc->password, studentID);  // Default password is student ID
                newAcc->role = role;
                newAcc->isLocked = 0;
                newAcc->failCount = 0;

                // Increase counts
                members->count++;
                accounts->count++;

                // Call save member to file function
                saveMembers(members);
                // Call save account to file function (Assume signature changed accordingly)
                saveAccounts(accounts);

                // Print success message
                uiSuccess("Member added successfully!\n");

                // Give back account info to user
                printf("\nAccount information for this member:\n");
                printf("Student ID: %s\n", newAcc->studentID);
                printf("Password: %s\n", newAcc->password);

                // Print success message
                uiSuccess("Member added successfully!\n");
                
            } else {
                printf("Member not added.\n");
            }
        } else {
            printf(
                "Student ID already exists! Please try again with a different "
                "ID.\n");
        }

        // ===== CONTINUE =====
        int choice;
        inputYesNo(&choice,
                   "\nDo you want to add another member?\n1: Yes\n0: No\n=> "
                   "Your choice: ");

        if (choice == 0) {
            continueAdd = 0;
        }
    }
}

void removeOneMember (MemberList *members, AccountList *accounts, ViolationList *violations, const char *id, const char *actorID){
    int mIndex = -1, vIndex = -1, aIndex = -1, actorIndex = -1;  // Reset position before find member
    // Find member by ID in member list
    mIndex = searchMemberByIdInM(members, id);
    actorIndex = searchMemberByIdInM(members, actorID);

    if (mIndex != -1) {
        // Show student
        printf("\nStudent found:\n");
        displayOneMemberInfo(members->data[mIndex]);

        if (actorIndex == -1) {
            printf("Actor not found!\n");
            return;
        }

        // Only BOD can remove BOD
        if (members->data[mIndex].role == 2 && members->data[actorIndex].role < 2){
            printf ("You are only Vice/Leader, you are not granted permission to remove BOD\n");
            return;
        }    
        else if (members->data[mIndex].role == 2 && members->data[actorIndex].role == 2  && checkTotalBOD (members)== 1){
            //Only can remove themself if they are not the last BOD
            printf ("You are the last BOD, can't remove yourself\n");
            return;
        }
        else {
            // Confirm to remove member
            int confirm;
            inputYesNo(&confirm, "\nRemove this member?\n1: Yes\n0: No\n=> Your choice: ");

            // Start to remove
            if (confirm == 1) {
                // Search member in violation list and account list (Assume signatures changed)
                aIndex = searchMemberByIdInA(accounts, id);

                // Remove in member list by shift left array
                for (int i = mIndex; i < members->count - 1; i++) {
                    members->data[i] = members->data[i + 1];
                }
                members->count--;

                // Remove in account list by shift left array if found
                for (int i = aIndex; i < accounts->count - 1; i++) {
                    accounts->data[i] = accounts->data[i + 1];
                }
                accounts->count--;
            
                // Loop through all violations to remove every violation of this member
                for (int i = 0; i < violations->count; i++) {
                    if (strcmp(violations->data[i].studentID, id) == 0) {
                        for (int j = i; j < violations->count - 1; j++) {
                            violations->data[j] = violations->data[j + 1];
                        }
                        violations->count--;
                        i--; // Check at this index again after shifting
                    }
                }

                // Call save member and account to file functions
                saveMembers(members);
                saveAccounts(accounts);
                saveViolations(violations);

                // Delete notification 
                deleteNotificationByMemberId(id); 

                // Print success message
                uiSuccess("Member removed successfully!\n");
            } 
            else {
                printf("Member not removed.\n");
            }
        }
    } 
    else {
        printf("Member not found!\n");
    }
}

// ===== Feature 2.2: REMOVE MEMBER =====
/*
Input ID => Find by ID => If found, show member info
=> Confirm to remove => If yes, remove by shift left array => Save to file
*/
void removeMember(MemberList* members, AccountList* accounts, ViolationList* violations, const char *actorID) {
    // Check if member list is empty
    if (members->count == 0) {
        printf("No members available to remove.\n");
        return;
    }

    char id[10];

    int continueRemove = 1;
    while (continueRemove) {
        inputStudentID(id, "Enter student ID to remove: ");

        removeOneMember(members, accounts, violations, id, actorID);

        int choice;

        inputYesNo(&continueRemove,
                   "\nDo you want to remove another member?\n1: Yes\n0: No\n=> "
                   "Your choice: ");
    }
}

// ===== Feature 2.3: UPDATE MEMBER =====
/*
Input ID => Find by ID => If found, show member info
=> ask which field want to update
=> input new value => confirm to update
=> If yes, update by assign new value to target member
=> Save to file
*/
void updateMember(MemberList* members, ViolationList* violations, const char *actorID) {
    // Check if member list is empty
    if (members->count == 0) {
        printf("No members available to update.\n");
        return;
    }

    char fullName[100];
    char email[100];
    char phoneNumber[11];
    char studentID[10];  // SE000000\0
    int team;            // 0 = Academic, 1 = Planning, 2 = HR, 3 = Media
    int role;            // 0 = Member, 1 = Leader/Vice, 2 = BOD

    int continueUpdate = 1;
    while (continueUpdate) {
        int mIndex = -1, actorIndex = -1;  // Reset position before find member
        inputStudentID(studentID, "Enter student ID to update: ");

        // Find member by ID
        mIndex = searchMemberByIdInM(members, studentID);
        actorIndex = searchMemberByIdInM(members, actorID);


        // If found
        if (mIndex != -1) {
            // Show student
            printf("\nStudent found:\n");
            displayOneMemberInfo(members->data[mIndex]);

            if (actorIndex == -1) {
                printf("Actor not found!\n");
                continue;
            }

            // Only BOD can update BOD
            if (members->data[mIndex].role == 2 && members->data[actorIndex].role < 2){
                printf ("You are only Vice/Leader, you are not granted permission to update BOD\n");
                continue;
            }    

            // Ask which field want to update
            int fieldChoice;
            do {
                printf("\nWhich field do you want to update?\n");
                printf(
                    "1: Name\n2: Email\n3: Phone Number\n4: Team\n5: Role\n");
                inputIntegerInRange(&fieldChoice, 1, 5,
                                    "=> Your choice (only 1-5): ");
                while (getchar() != '\n');

            } while (fieldChoice < 1 || fieldChoice > 5);

            switch (fieldChoice) {
                case 1:
                    inputMemberName(fullName, "Enter New Name: ");
                    break;
                case 2:
                    inputMemberEmail(email, "Enter New Email: ");
                    break;
                case 3:
                    inputMemberPhone(phoneNumber, "Enter New Phone Number: ");
                    break;
                case 4:
                    inputMemberTeam(&team, "0. Academic\n1. Planning\n2. HR\n3. Media\nEnter New Team (0-3): ");
                    break;
                case 5:
                    inputMemberRole(&role, "0. Member\n1. Leader/Vice\n2. BOD\nEnter New Role (0-2): ");
                    break;
            }

            // Confirm to update member
            int confirm;
            inputYesNo(
                &confirm,
                "\nUpdate this member?\n1: Yes\n0: No\n=> Your choice: ");

            if (confirm == 1) {
                // Use pointer to manipulate the data block directly, avoiding shallow copy
                Member* targetMem = &members->data[mIndex];

                switch (fieldChoice) {
                    case 1:
                        strcpy(targetMem->fullName, fullName);
                        break;
                    case 2:
                        strcpy(targetMem->email, email);
                        break;
                    case 3:
                        strcpy(targetMem->phoneNumber, phoneNumber);
                        break;
                    case 4:
                        targetMem->team = team;
                        break;
                    case 5: {
                        if (role == 2 && members->data[actorIndex].role < 2) {
                            printf ("You are not granted permission to promote this member to BOD.\n");
                            break;
                        }
                        if (members->data[mIndex].role == 2 
                            && members->data[actorIndex].role < 2){
                            printf ("You are only Vice/Leader, you are not granted permission to change BOD role.\n");
                            break;
                        }
                        if (members->data[mIndex].role == 2 
                            && members->data[actorIndex].role == 2  
                            && checkTotalBOD (members)== 1){
                            // The last BOD cannot downgrade themselves.
                            printf ("You are the last BOD, cannot change your role to another role.\n");
                            break;
                        }
                        int oldRole = targetMem->role; // Save old role before assign new role
                        targetMem->role = role;        // Assign new role

                        // If member change role (either upgrade or downgrade)
                        if ((oldRole == 0 && role > 0) || (oldRole > 0 && role == 0)) {
                            // Replace new fines for all unpaid and not pending violation of this member
                            for (int i = 0; i < violations->count; i++) {
                                Violation* vPtr = &violations->data[i];
                                if (strcmp(vPtr->studentID, studentID) == 0 &&
                                    vPtr->isPaid == NOT_PAY &&
                                    vPtr->owner->isPending == NOT_PENDING &&
                                    vPtr->fine != 0) {
                                    
                                    double newFine = calculateFine(role, vPtr->reason);
                                    vPtr->fine = newFine;
                                }
                            }
                            // Update total fine for this member after change role
                            updateMemberTotalFine(members, violations, studentID);
                        }

                        saveViolations(violations);
                        break;
                    }
                }
                saveMembers(members);

            } else {
                printf("Member not updated.\n");
            }
        } else {
            printf("Member not found!\n");
        }

        // ===== CONTINUE =====
        int choice;
        inputYesNo(&choice,
                   "\nDo you want to update another member?\n1: Yes\n0: "
                   "No\n=>Your choice: ");

        if (choice == 0) {
            continueUpdate = 0;
        }
    }
}

Member* getMemberById(const char* studentId, MemberList* members) {
    for (int i = 0; i < members->count; i++) {
        if (strcmp(members->data[i].studentID, studentId) == 0) {
            // Return pointer directly to the array's memory location
            return &members->data[i];
        }
    }
    return NULL;
}


