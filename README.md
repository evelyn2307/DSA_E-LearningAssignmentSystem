# E-Learning Assignment System (C++)

## 📌 System Overview
The **E-Learning Assignment System** is a **console-based application** developed using **C++** as a mini project for the *Data Structures and Algorithms* course.  
The system is designed to manage assignment creation, submission, grading, and student ranking in a digital learning environment.

It supports two user roles — **Student** and **Lecturer** — where each role is provided with different system functionalities. The system applies multiple **data structures** to efficiently organize data and simulate real-world e-learning workflows.

---

## 🎯 Objectives
- To apply **Data Structures and Algorithms** concepts in a practical system
- To manage assignment submissions digitally
- To demonstrate efficient data organization using appropriate data structures
- To enhance problem-solving and modular programming skills in C++

---

## 🧠 Data Structures & Algorithms Used

### 1. Linked List
- Used to store assignment submissions dynamically
- Allows efficient insertion of new submissions without shifting existing data
- Each node stores submission details and a pointer to the next node


### 2. Queue (FIFO)
- Used to manage assignment submissions in the order they are received
- Follows the First-In-First-Out (FIFO) principle
- Ensures fairness when lecturers process submissions


### 3. Stack (LIFO)
- Used to store assignment creation history
- Follows the Last-In-First-Out (LIFO) principle
- Allows lecturers to undo the most recently created assignment


### 4. Binary Search Tree (BST)
- Used to store assignments based on their titles
- Enables fast searching of assignments by title
- Organizes data in a hierarchical structur

### 5. Sorting Algorithms
- Used to organize assignments by due date
- Used to rank students based on total marks
- Rankings are sorted in descending order (highest score first)

---

## ⚙️ System Features

### 👨‍🎓 Student Functions
- Login as a student
- View available assignments
- Search assignments using BST
- Submit assignments digitally
- View grades after lecturer evaluation
- View class ranking based on total scores

### 👩‍🏫 Lecturer Functions
- Login as a lecturer
- Create new assignments (title, description, due date, max score)
- Automatically sort assignments by due date
- View assignment creation history (LIFO)
- Undo recently created assignments
- View assignment submissions in FIFO order
- Grade assignments with marks and comments
- View student rankings based on total marks

---

## ▶️ How to Compile and Run

Ensure a C++ compiler (e.g. `g++`) is installed.

```bash
g++ main.cpp -o elearning-system
./elearning-system
