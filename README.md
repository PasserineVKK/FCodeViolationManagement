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

## 2. Functions & Error Handling
* **Function Structure:** Functions should focus on a single responsibility.
* **Return Values:** Functions that perform actions (Save, Add, Delete) must return an `int` error code:
    * `1`: Success.
    * `0`: General Error 
    * `-1`: Duplicate ID.
    * `-2`: File Access Error.
    * `-3`: Input Invalid.


## 3. Memory Management
* **Static Allocation:** Use static arrays with predefined sizes for simplicity.
* **Global Variables:** Avoid global variables unless absolutely necessary (e.g., the main database array).

## 4. Git Workflow
* **Branching Strategy:**
    * `main`: Stable code only.
    * `feature/<feature-name>`: Individual work branches.
* **Atomic Commits:** Each commit should represent a single, logical change.
* **Commit Message Format:** `[Action] Description` (In English).
    * `[Add] member struct and header guards`
    * `[Fix] login validation logic`
    * `[Update] README with installation steps`

## 5. Documentation & Comments
* **Language:** All comments and documentation must be in English.
* **Header Guards:** Every `.h` file must include `#ifndef`, `#define`, and `#endif`.
* **Code Comments:** Explain *why* a complex logic is used, not just *what* the code does.

## 6. Data Management
* **Local Data:** Use your own `.dat` files for testing. 
* DO NOT commit `.dat` files to the remote repository. Ensure `data/*.dat` is in `.gitignore`.
