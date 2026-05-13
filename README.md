# F-Code Violation Management System (F-VMS)

The F-Code Violation Management System is a terminal-based solution developed in C to digitize the tracking of member violations, fines, and club status for the F-Code Academic Club at FPT University HCMC. This system replaces manual record-keeping with a persistent, file-based database.

## Project Overview
* Team Name: Let Me C.
* Team Members: 
    - Văng Khánh Khuyên
    - Hồ Lê Thiên An
    - Bùi Phước Trọng
    - Nguyễn Văn Bình
    - Tạ Anh Đức.
* Development Period: April 17, 2026 – May 17, 2026.
* Mentor: Nguyễn Huy Phong.
* Platform: Terminal-based (Command Line Interface).
* Language: Standard C.

## Key Features

### 1. Authentication and Security
* Role-based access control:
    * Admin:
    - BOB 
    - Leader
    - Vice
    * Member: normal club member
* Secure login with account locking
    - Failed login 3 times the first time: account locked for 5 minutes
    - Incorrect login 3 times the next time: account locked for 60 minutes
    - Incorrect login 3 times the next time: account permanently locked (can only wait for admin to unlock)
    - During the waiting period, you can choose to view the countdown timer or exit the program.
    - Account suspension time is calculated based on actual time.
* Password management: 
    - Members can change their own passwords.
    - Admins can reset members' passwords to the default password (which is the student ID).
    - When members reset their passwords, they are required to enter their old passwords. When admins reset a member's password, this is not required.
    - The new password will be entered twice; the second attempt ensures that the new password is not entered incorrectly.

### 2. Member Management (Admin Only)
* Manage up to 1,000 club members with unique Student IDs (MSSV - student ID).
* Categorize members into 4 departments: 
    - Planning
    - HR
    - Media
    - Academic
* Categorize members into 3 role:
    - Member
    - Leader/Vice
    - BOD
* Full CRUD operations (Create, Read, Update, Delete) for member profiles.

### 3. Violation and Penalty System
* Automated Fines: Calculate penalties based on role (20,000d for Members, 50,000d for Leaders/BOD).
* Attendance Tracking: Automatically flag members for "Out Club" status after 3 consecutive unexcused absences.
* Immediate Enforcement: Instant "Out Club" processing for violence violations.

### 4. Reporting and Statistics
* View personal violation history and outstanding debt.
* Generate club-wide reports filtered by department or payment status.
* Export violation summaries to .txt files with timestamps.

### 5. Member menu
    1. 🙍 **View Profile**: Display personal account information and member details.  
    2. 📜 **View Violation History**: Review all recorded violations of this member.  
    3. 💰 **View Total Unpaid Fines**: Check the total amount of unpaid penalties of this member.  
    4. 👥 **View Club Member List**: Display the complete list of club members.  
    5. 🔑 **Reset Your Password**: 
        - Change the current account password securely.
        - Must input old password os this member account.
        - The new password will be entered twice; the second attempt ensures that the new password is not entered incorrectly.  
    6. 🔔 **View Notifications**: 
        - Display all available announcements
        - Display all notifications of this member.  
    7. 🚪 **Log Out**: Sign out from the current account session.  
    8. 🛑 **Exit**: Close the application safely.  

    *9. 🔄 **Switch to Admin Menu**: Navigate to the administrator management interface.  
        > Only available for users with admin privileges.


### 6. Admin menu

  ### 👥 Member Management

    1. ➕ **Add New Member**:
        - Add a member to the club.
        - You will need to fill in the member's full information.
        - After adding, the system will automatically create an account for that person.
        - The default password when added is the student ID.
    2. ✏️ **Edit Member Information**: 
        - Update existing member details.  
        - Enter student ID to identify the person whose information needs editing.
        - Only basic information can be changed; student ID and password cannot be changed.
        - When changing roles, any outstanding fines will be updated according to the current role's unit price, and the total amount due will also be updated.
    3. ❌ **Remove Member**: 
        - Delete a member account from the system.  
        - Enter the student ID to identify the person to be removed.
    4. ⚠️ **Check Warning/Kick List**:
        - View members who are warned (warning list) or waiting to remove (kick list).  
        - Members who miss two meetings will be added to the warning list.
        - Members who miss a meeting a third time or engage in violent behavior will be moved to the kick list.
        - Members will not be directly removed from the kick list; they will wait for admin approval. 
        - The admin will then enter the student ID of the member on the kick list to permanently remove that member.
        - Members on the kick list can only use the member menu. 
        - Even admins, if placed on the kick list, will have their access restricted to the member menu.
    5. 📋 **View Member in Sorted List**: Display members with sorting options.  
    6. 🔑 **Change Member's Password**: 
        - Update a member password securely. 
        - Enter the student ID to find out which member needs their password changed.
        - If the admin changes their own password, you don't need to enter the student ID.
        - Admins can reset members' passwords to the default password (which is the student ID).
        - When admins reset a member's password, admins don't need to enter the old password.
        - The new password will be entered twice; the second attempt ensures that the new password is not entered incorrectly.
    7. 🔓 **Unlock Member Account**: 
        - Unlock accounts that were previously locked.  
        - Enter your student ID to unlock.
        - Only can unlock permanent locked account


  ### 🚨 Violation Management
    8.  📝 **Record New Violation**: 
        - Add a new violation record. 
        - Users are required to enter their student ID and reason
        - The notes section can be omitted. 
        - The remaining information will be automatically retrieved by the system to calculate the penalty fee based on the role.
        - If a member misses a meeting for the third time or engages in violent behavior, the penalty will be "kick", and no penalty fee will be charged.
        - Violators will be automatically assigned ID numbers using VIOxxx.
        - xxx is assigned using base-16 to increase storage capacity (higher base systems may be updated in the future if needed).
    9.  💰 **Mark Fine as Paid**: Input violation ID to update fine payment status.  
    10. 📂 **View Violation List**: Display all recorded violations.  
    11. 📊 **Statistics by Department**: 
        - Generate role-based statistics.
        - Including paid and unpaid data, and the total paid and unpaid data for each role.  
    12. 🕒 **View Violations by Time Range**: 
        - Filter violations within a specific period.
        - Data is entered by the user.
        - It is not necessary to enter all the figures.
        - Any data not entered will be automatically filled in using default values.
    13. 🗑️ **Delete Violation**: 
        - Remove a violation record from the system.  
        - Users enter the violation ID to remove the specific violation.
    14. 🔄 **Change Violation Filter/Sort Settings**: Customize filtering and sorting options.  
    15. 📤 **Export Violation Report**: 
        - Export violation reports in .txt file format.
        - With timestamps attached.  


  ### 🔔 Notification Management
    16. ➕ **Add New Notification**: Create a notification announcement.  
    17. ✏️ **Update Notification**: Edit notification content.  
    18. ❌ **Delete Notification**: Remove a notification from the system.  
    19. 📢 **Show All Notifications**: Display all available notifications.  


  ### ⚙️ System Operations

    20. 🚪 **Log Out**: Sign out from the current account session.  
    21. 🛑 **Exit**: Close the application safely.  
    22. 🔄 **Switch to Member Menu**: Navigate to the member interface menu.  

## Project Structure
The project follows a modular design to ensure maintainability and team collaboration:
* /src: Implementation files (.c).
* /include: Header files (.h) containing structs and function prototypes.
* /data: Binary database files (.dat) for persistent storage.

## Installation and Compilation
To run the system, ensure you have a C compiler (like GCC) installed.

1. Clone the repository:
   git clone [https://github.com/PasserineVKK/FCodeViolationManagement.git]

2. Compile the source code:
   `gcc src/*.c src/view/*.c -Iinclude -o main.exe`

3. Execute the program:
  `./main.exe`

## Contribution Guidelines (Internal)
* Coding Standards: Use English for all variable and function names.
* Git Workflow: 
    * Commit early and often with meaningful messages.
    * One feature per branch; ensure code is tested before merging to main.
* Data Integrity: Always use the provided File I/O modules to ensure data is saved after every transaction.

# Coding Conventions 

## 1. Naming Conventions
* **Functions & Variables:** Use `camelCase`.
    * *Example:* `addMember()`, `studentId`, `isPaid`.
* **Constants & Macros:** Use `UPPER_SNAKE_CASE`.
    * *Example:* `MAX_MEMBERS`, `BASE_FINE_MEMBER`.
* **Structs:** Use `PascalCase`.
    * *Example:* `Member`, `ViolationRecord`.



## 3. Memory Management
* **Static Allocation:** Use static arrays with predefined sizes (Members, Violations) for simplicity.
* **Dynamic Allocation:** Use dynamic arrays with unpredefined sizes (Violations)



## 4. Git Workflow



### 4.1. Branching Strategy

* **`master`**: The primary branch containing production-ready code. Direct commits to this branch are strictly prohibited.
* **`develop`**: The integration branch for features. All completed features are merged here for testing before moving to `master`.
* **`feature/`**: Used for developing new features or enhancements.
    * *Naming Convention:* `feature/<feature-name>`
* **`fix/`**: Used for quick patches to production bugs.
    * *Naming Convention:* `fix/<short-description>`
* **`refactor/`**: Used for code restructuring without changing functionality.

### 4.2. Development Process

1.  **Sync with Remote:**
    Always start by updating your local `develop` branch to avoid conflicts later.
    ```bash
    git checkout develop
    git pull origin develop
    ```

2.  **Create a Feature Branch:**
    ```bash
    git checkout -b feature/validate
    ```

3.  **Work and Commit:**
    Make small, frequent commits using **Conventional Commits** standards:
    * `feat`: A new feature
    * `fix`: A bug fix.
    * `docs`: Documentation changes only.
    * `style`: Changes that do not affect the meaning of the code (white-space, formatting).
    * `update`: Changes of existed feature, for example. the way to display data in terminal
    

    ```bash
    git add .
    git commit -m "feat: validate input data"
    ```

4.  **Push to Remote:**
    ```bash
    git push origin feature/validate
    ```

5.  **Open a Pull Request (PR):**
    * Target branch: `develop` ← Source branch: `feature/validate`.
    * Assign at least one **Reviewer**.
    * Link the PR to the relevant task/issue ID.

6.  **Code Review & Merge:**
    * Address any comments from reviewers.
    * Once approved, the branch will be merged.

### 4.3. Golden Rules

* **Never** use `git push --force` on shared branches (`main`, `develop`).
* **Pull before you push:** Always sync with the remote repository to handle conflicts locally.
* **Meaningful Commits:** Ensure commit messages describe *what* changed and try to explain *why*.

## 5. Documentation & Comments
* **Language:** All comments and documentation must be in English.
* **Header Guards:** Every `.h` file must include `#ifndef`, `#define`, and `#endif`.
* **Code Comments:** Explain *why* a complex logic is used, not just *what* the code does.

## 6. Data Management
* **Local Data:** Use your own `.dat` files for testing. 

