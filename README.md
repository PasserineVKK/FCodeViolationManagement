# F-Code Violation Management System (F-VMS)

The F-Code Violation Management System is a terminal-based solution developed in C to digitize the tracking of member violations, fines, and club status for the F-Code Academic Club at FPT University HCMC. This system replaces manual record-keeping with a persistent, file-based database.

## Project Overview
* Team Name: Let Me C.
* Team Members: Văng Khánh Khuyên, Hồ Lê Thiên An, Bùi Phước Trọng, Nguyễn Văn Bình, Tạ Anh Đức.
* Development Period: April 17, 2026 – May 17, 2026.
* Mentor: Nguyễn Huy Phong.
* Platform: Terminal-based (Command Line Interface).
* Language: Standard C.

## Key Features

### 1. Authentication and Security
* Role-based access control (Admin/Member).
* Secure login with account locking after 3 failed attempts.
* Password management (change and reset capabilities).

### 2. Member Management (Admin Only)
* Manage up to 1,000 club members with unique Student IDs (MSSV).
* Categorize members into 4 departments: Planning, HR, Media, and Academic.
* Full CRUD operations (Add, Edit, Delete) for member profiles.

### 3. Violation and Penalty System
* Automated Fines: Calculate penalties based on role (20,000d for Members, 50,000d for Leaders/BCN).
* Attendance Tracking: Automatically flag members for "Out Club" status after 3 consecutive unexcused absences.
* Immediate Enforcement: Instant "Out Club" processing for violence violations.

### 4. Reporting and Statistics
* View personal violation history and outstanding debt.
* Generate club-wide reports filtered by department or payment status.
* Export violation summaries to .txt files with timestamps.

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
   gcc src/*.c -I include -o fcode_manager

3. Execute the program:
   ./fcode_manager

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
* **Filenames:** Use lowercase with underscores if needed.
    * *Example:* `fileio.c`, `auth.h`.


## 3. Memory Management
* **Static Allocation:** Use static arrays with predefined sizes for simplicity.
* **Global Variables:** Avoid global variables unless absolutely necessary (e.g., the main database array).


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
    * Once approved, the branch will be merged (preferably using **Squash and Merge**).
    * Delete the feature branch locally and on remote after a successful merge.

### 4.3. Golden Rules

* **Never** use `git push --force` on shared branches (`main`, `develop`).
* **Pull before you push:** Always sync with the remote repository to handle conflicts locally.
* **Meaningful Commits:** Ensure commit messages describe *what* changed and *why*.
* **No Broken Code:** Never merge a PR that fails build tests or contains syntax errors.

## 5. Documentation & Comments
* **Language:** All comments and documentation must be in English.
* **Header Guards:** Every `.h` file must include `#ifndef`, `#define`, and `#endif`.
* **Code Comments:** Explain *why* a complex logic is used, not just *what* the code does.

## 6. Data Management
* **Local Data:** Use your own `.dat` files for testing. 
* DO NOT commit `.dat` files to the remote repository. Ensure `data/*.dat` is in `.gitignore`.
