#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ============== STRUCTURES ==============

struct Assignment {
    string assignmentID;
    string title;
    string dueDate;
    string description;
    int maxScore;
    Assignment* next;
};

struct Submission {
    string submissionID;
    string studentID;
    string studentName;
    string assignmentID;
    string assignmentTitle;
    string submittedDate;
    string filePath;
    string status; 
    int score;
    string feedback;
    Submission* next;
};

struct Student {
    string studentID;
    string name;
    string email;
    int totalScore;
    int submissionCount;
};

struct StackNode {
    Assignment* assignment;
    StackNode* next;
};

struct BSTNode {
    string key;      
    Assignment* value; 
    BSTNode* left;
    BSTNode* right;
};

// ============== BST CLASS ==============
class BST {
private:
    BSTNode* root;

    void insert(BSTNode*& node, string key, Assignment* value) {
        if (!node)
            node = new BSTNode{key, value, nullptr, nullptr};
        else if (key < node->key)
            insert(node->left, key, value);
        else if (key > node->key)
            insert(node->right, key, value);
    }

    Assignment* search(BSTNode* node, string key) {
        if (!node) return nullptr;
        if (key == node->key) return node->value;
        else if (key < node->key) return search(node->left, key);
        else return search(node->right, key);
    }

    BSTNode* deleteNode(BSTNode* node, string key) {
        if (!node) return nullptr;
        
        if (key < node->key)
            node->left = deleteNode(node->left, key);
        else if (key > node->key)
            node->right = deleteNode(node->right, key);
        else {
            if (!node->left) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            } else {
                BSTNode* succ = node->right;
                while (succ->left) succ = succ->left;
                node->key = succ->key;
                node->value = succ->value;
                node->right = deleteNode(node->right, succ->key);
            }
        }
        return node;
    }

public:
    BST() : root(nullptr) {}
    
    void insertItem(string key, Assignment* value) { insert(root, key, value); }
    Assignment* searchByTitle(string key) { return search(root, key); }
    void deleteItem(string key) { root = deleteNode(root, key); }
};

// ============== STACK CLASS (for Undo) ==============
class AssignmentStack {
private:
    StackNode* top;
    int size;

public:
    AssignmentStack() {
        top = nullptr;
        size = 0;
    }

    void push(Assignment* assignment) {
        StackNode* newNode = new StackNode();
        newNode->assignment = assignment;
        newNode->next = top;
        top = newNode;
        size++;
    }

    Assignment* pop() {
        if (isEmpty()) {
            return nullptr;
        }
        
        StackNode* temp = top;
        Assignment* assignment = top->assignment;
        top = top->next;
        delete temp;
        size--;
        return assignment;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    int getSize() {
        return size;
    }

    void displayStack() {
        if (isEmpty()) {
            cout << "\nNo assignments in history.\n";
            return;
        }

        cout << "\n=== ASSIGNMENT CREATION HISTORY (Stack - LIFO) ===\n";
        cout << left
             << setw(12) << "ID"
             << setw(25) << "Title"
             << setw(15) << "Due Date" << endl;
        cout << string(52, '-') << endl;

        StackNode* temp = top;
        while (temp != nullptr) {
            cout << left
                 << setw(12) << temp->assignment->assignmentID
                 << setw(25) << temp->assignment->title
                 << setw(15) << temp->assignment->dueDate << endl;
            temp = temp->next;
        }
        cout << endl;
    }

    ~AssignmentStack() {
        while (!isEmpty()) {
            pop();
        }
    }
};

// ============== ASSIGNMENT LINKED LIST ==============
class AssignmentLinkedList {
private:
    Assignment* head;
    int count;
    BST bst; 
    AssignmentStack stack; 

    string generateID() {
        count++;
        return "ASSG" + to_string(1000 + count);
    }

public:
    AssignmentLinkedList() {
        head = nullptr;
        count = 0;
    }

    // Insert assignment
    void insertAssignment() {
        Assignment* newAssg = new Assignment();
        
        cout << "\n=== CREATE NEW ASSIGNMENT ===\n";
        newAssg->assignmentID = generateID();
        cout << "Assignment ID: " << newAssg->assignmentID << endl;
        
        cin.ignore();
        cout << "Title: ";
        getline(cin, newAssg->title);
        
        cout << "Description: ";
        getline(cin, newAssg->description);
        
        cout << "Due Date (YYYY-MM-DD): ";
        getline(cin, newAssg->dueDate);
        
        cout << "Max Score: ";
        cin >> newAssg->maxScore;
        
        newAssg->next = nullptr;

       
        bst.insertItem(newAssg->title, newAssg);

        stack.push(newAssg);

        if (head == nullptr || newAssg->dueDate < head->dueDate) {
            newAssg->next = head;
            head = newAssg;
        } 
        else {
            Assignment* current = head;
            while (current->next != nullptr && current->next->dueDate < newAssg->dueDate) {
                current = current->next;
            }
            newAssg->next = current->next;
            current->next = newAssg;
        }
        
        cout << "\n Assignment created successfully!\n";
        cout << "  (Use 'Undo Last Creation' to remove this assignment)\n";
    }

    // Stack operation
    void undoLastCreation() {
        if (stack.isEmpty()) {
            cout << "\nNo assignments to undo.\n";
            return;
        }

        Assignment* lastAssg = stack.pop();
        
        Assignment* temp = head;
        Assignment* prev = nullptr;

        while (temp != nullptr && temp != lastAssg) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Error: Assignment not found in list.\n";
            return;
        }

        if (prev == nullptr)
            head = temp->next;
        else
            prev->next = temp->next;

        bst.deleteItem(lastAssg->title);

        cout << "\n✓ Undo successful! Assignment '" << lastAssg->title << "' removed.\n";
        
        delete lastAssg;  
    }

    void displayCreationHistory() {
        stack.displayStack();
    }

    void displayAll() {
        if (head == nullptr) {
            cout << "\nNo assignments available.\n";
            return;
        }

        cout << "\n=== ALL ASSIGNMENTS (Linked List - Sorted by Due Date) ===\n";
        cout << left
             << setw(12) << "ID"
             << setw(25) << "Title"
             << setw(15) << "Due Date"
             << setw(10) << "Max Score" << endl;
        cout << string(62, '-') << endl;

        Assignment* temp = head;
        while (temp != nullptr) {
            cout << left
                 << setw(12) << temp->assignmentID
                 << setw(25) << temp->title
                 << setw(15) << temp->dueDate
                 << setw(10) << temp->maxScore << endl;
            temp = temp->next;
        }
        cout << endl;
    }

    // Search using BST 
    Assignment* searchByTitle(string title) {
        return bst.searchByTitle(title);
    }

    // Search by ID
    Assignment* searchByID(string id) {
        Assignment* temp = head;
        while (temp != nullptr) {
            if (temp->assignmentID == id) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void displayDetails(Assignment* assg) {
        if (assg == nullptr) {
            cout << "Assignment not found.\n";
            return;
        }

        cout << "\n=== ASSIGNMENT DETAILS ===\n";
        cout << "ID: " << assg->assignmentID << endl;
        cout << "Title: " << assg->title << endl;
        cout << "Description: " << assg->description << endl;
        cout << "Due Date: " << assg->dueDate << endl;
        cout << "Max Score: " << assg->maxScore << endl;
    }

    ~AssignmentLinkedList() {
        Assignment* current = head;
        while (current != nullptr) {
            Assignment* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

// ============== SUBMISSION QUEUE ==============
class SubmissionQueue {
private:
    Submission* front;
    Submission* rear;
    int count;

    string generateID() {
        count++;
        return "SUB" + to_string(10000 + count);
    }

    string getCurrentDate() {
        return "2025-01-11";
    }

public:
    SubmissionQueue() {
        front = nullptr;
        rear = nullptr;
        count = 0;
    }

    // Enqueue
    void enqueue(Student student, Assignment* assignment) {
        if (assignment == nullptr) {
            cout << "Invalid assignment.\n";
            return;
        }

        Submission* newSub = new Submission();
        
        newSub->submissionID = generateID();
        newSub->studentID = student.studentID;
        newSub->studentName = student.name;
        newSub->assignmentID = assignment->assignmentID;
        newSub->assignmentTitle = assignment->title;
        newSub->submittedDate = getCurrentDate();
        
        cout << "Enter file path/description: ";
        cin.ignore();
        getline(cin, newSub->filePath);
        
        newSub->status = "Pending";
        newSub->score = 0;
        newSub->feedback = "Not graded yet";
        newSub->next = nullptr;

        // Queue operation (FIFO)
        if (rear == nullptr) {
            front = rear = newSub;
        } else {
            rear->next = newSub;
            rear = newSub;
        }

        cout << "\n✓ Assignment submitted successfully!\n";
        cout << "Submission ID: " << newSub->submissionID << endl;
    }

    bool isDuplicate(string studentID, string assignmentID) {
        Submission* temp = front;
        while (temp != nullptr) {
            if (temp->studentID == studentID && temp->assignmentID == assignmentID) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    void displayPending() {
        if (front == nullptr) {
            cout << "\nNo pending submissions.\n";
            return;
        }

        cout << "\n=== PENDING SUBMISSIONS (Queue - FIFO Order) ===\n";
        cout << left
             << setw(12) << "Sub ID"
             << setw(20) << "Student"
             << setw(25) << "Assignment"
             << setw(15) << "Date" << endl;
        cout << string(72, '-') << endl;

        Submission* temp = front;
        while (temp != nullptr) {
            if (temp->status == "Pending") {
                cout << left
                     << setw(12) << temp->submissionID
                     << setw(20) << temp->studentName
                     << setw(25) << temp->assignmentTitle
                     << setw(15) << temp->submittedDate << endl;
            }
            temp = temp->next;
        }
        cout << endl;
    }

    void displayByStudent(string studentID) {
        Submission* temp = front;
        bool found = false;

        cout << "\n=== MY SUBMISSIONS ===\n";
        cout << left
             << setw(25) << "Assignment"
             << setw(15) << "Date"
             << setw(12) << "Status"
             << setw(8) << "Score"
             << setw(20) << "Feedback" << endl;
        cout << string(80, '-') << endl;

        while (temp != nullptr) {
            if (temp->studentID == studentID) {
                cout << left
                     << setw(25) << temp->assignmentTitle
                     << setw(15) << temp->submittedDate
                     << setw(12) << temp->status
                     << setw(8) << temp->score
                     << setw(20) << temp->feedback << endl;
                found = true;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "No submissions found.\n";
        }
        cout << endl;
    }

    void gradeSubmission(string submissionID) {
        Submission* temp = front;
        
        while (temp != nullptr) {
            if (temp->submissionID == submissionID) {
                if (temp->status == "Graded") {
                    cout << "This submission is already graded.\n";
                    return;
                }

                cout << "\n=== GRADING SUBMISSION ===\n";
                cout << "Student: " << temp->studentName << " (" << temp->studentID << ")\n";
                cout << "Assignment: " << temp->assignmentTitle << endl;
                cout << "File: " << temp->filePath << endl;
                
                cout << "\nEnter Score: ";
                cin >> temp->score;
                cin.ignore();
                
                cout << "Enter Feedback: ";
                getline(cin, temp->feedback);
                
                temp->status = "Graded";
                
                cout << "\n✓ Submission graded successfully!\n";
                return;
            }
            temp = temp->next;
        }
        
        cout << "Submission not found.\n";
    }

    //Sorting algorithm
    void displayStudentRankings() {
        if (front == nullptr) {
            cout << "\nNo submissions to rank.\n";
            return;
        }

        Student students[100];  
        int studentCount = 0;

        Submission* temp = front;
        while (temp != nullptr) {
            if (temp->status == "Graded") {
                int index = -1;
                for (int i = 0; i < studentCount; i++) {
                    if (students[i].studentID == temp->studentID) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    students[studentCount].studentID = temp->studentID;
                    students[studentCount].name = temp->studentName;
                    students[studentCount].totalScore = temp->score;
                    students[studentCount].submissionCount = 1;
                    studentCount++;
                } else {
                    students[index].totalScore += temp->score;
                    students[index].submissionCount++;
                }
            }
            temp = temp->next;
        }

        if (studentCount == 0) {
            cout << "\nNo graded submissions yet.\n";
            return;
        }

        // Bubble Sort 
        for (int i = 0; i < studentCount - 1; i++) {
            for (int j = 0; j < studentCount - i - 1; j++) {
                if (students[j].totalScore < students[j + 1].totalScore) {
                    Student tempStudent = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = tempStudent;
                }
            }
        }

        cout << "\n=== STUDENT RANKINGS (Sorted by Total Score) ===\n";
        cout << left
             << setw(6) << "Rank"
             << setw(12) << "Student ID"
             << setw(20) << "Name"
             << setw(12) << "Total Score"
             << setw(12) << "Assignments"
             << setw(10) << "Average" << endl;
        cout << string(72, '-') << endl;

        for (int i = 0; i < studentCount; i++) {
            float average = (float)students[i].totalScore / students[i].submissionCount;
            cout << left
                 << setw(6) << (i + 1)
                 << setw(12) << students[i].studentID
                 << setw(20) << students[i].name
                 << setw(12) << students[i].totalScore
                 << setw(12) << students[i].submissionCount
                 << fixed << setprecision(2) << average << endl;
        }
        cout << endl;
    }

    ~SubmissionQueue() {
        Submission* current = front;
        while (current != nullptr) {
            Submission* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

// ============== GLOBAL OBJECTS ==============
AssignmentLinkedList assignmentList;
SubmissionQueue submissionQueue;
Student currentStudent;

// ============== MENU FUNCTIONS ==============

void studentLogin() {
    cout << "\n=== STUDENT LOGIN ===\n";
    cout << "Enter Student ID: ";
    cin >> currentStudent.studentID;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, currentStudent.name);
    cout << "Enter Email: ";
    getline(cin, currentStudent.email);
    
    cout << "\nWelcome, " << currentStudent.name << "!\n";
}

void studentMenu() {
    studentLogin();
    
    int choice;
    string id, title;
    
    do {
        cout<<endl;
        cout << "_____________________________________\n";
        cout << "|            STUDENT MENU            | \n";
        cout << "|____________________________________|\n";
        cout << "| 1. View All Assignments            |\n";
        cout << "| 2. Search Assignment (BST)         |\n";
        cout << "| 3. Submit Assignment (Queue)       |\n";
        cout << "| 4. View My Submissions             |\n";
        cout << "| 5. View Rankings (Sorting)         |\n";
        cout << "| 6. Logout                          |\n";
        cout << "|____________________________________|\n";
        cout << "Choice: ";
        cin >> choice;
        system("cls");

        switch (choice) {
            case 1:
                assignmentList.displayAll();
                break;
                
            case 2:
                cout << "\nSearch by:\n1. Title\n2. ID\nChoice: ";
                {
                    int searchChoice;
                    cin >> searchChoice;
                    cin.ignore();
                    
                    if (searchChoice == 1) {
                        cout << "Enter Assignment Title: ";
                        getline(cin, title);
                        assignmentList.displayDetails(assignmentList.searchByTitle(title));
                    } else {
                        cout << "Enter Assignment ID: ";
                        getline(cin, id);
                        assignmentList.displayDetails(assignmentList.searchByID(id));
                    }
                }
                break;
                
            case 3: {
                assignmentList.displayAll();
                cout << "Enter Assignment ID to submit: ";
                cin >> id;
                
                Assignment* assg = assignmentList.searchByID(id);
                if (assg == nullptr) {
                    cout << "Assignment not found.\n";
                    break;
                }
                
                if (submissionQueue.isDuplicate(currentStudent.studentID, id)) {
                    cout << "You already submitted this assignment.\n";
                    break;
                }
                
                submissionQueue.enqueue(currentStudent, assg);
                break;
            }
                
            case 4:
                submissionQueue.displayByStudent(currentStudent.studentID);
                break;
                
            case 5:
                submissionQueue.displayStudentRankings();
                break;
                
            case 6:
                cout << "Logging out...\n";
                break;
                
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 6);
}

void lecturerMenu() {
    int choice;
    string id;
    
    do {
    	cout<<endl;
        cout << "______________________________________\n";
        cout << "|     -------LECTURER MENU------      |\n";
        cout << "|_____________________________________|\n";
        cout << "| 1. Create Assignment                |\n";
        cout << "| 2. Undo Last Creation               |\n";
        cout << "| 3. View Creation History            |\n";
        cout << "| 4. View All Assignments             |\n";
        cout << "| 5. Search Assignment                |\n";
        cout << "| 6. View Pending Submissions         |\n";
        cout << "| 7. Grade Submission                 |\n";
        cout << "| 8. View Student Rankings            |\n";
        cout << "| 9. Logout                           |\n";
        cout << "|_____________________________________|\n";
        cout << "Choice: ";
        cin >> choice;
        system("cls");

        switch (choice) {
            case 1:
                assignmentList.insertAssignment();
                break;
                
            case 2:
                assignmentList.undoLastCreation();
                break;
                
            case 3:
                assignmentList.displayCreationHistory();
                break;
                
            case 4:
                assignmentList.displayAll();
                break;
                
            case 5: {
                string title;
                cout << "Enter Assignment Title: ";
                cin.ignore();
                getline(cin, title);
                assignmentList.displayDetails(assignmentList.searchByTitle(title));
                break;
            }
                
            case 6:
                submissionQueue.displayPending();
                break;
                
            case 7:
                submissionQueue.displayPending();
                cout << "Enter Submission ID to grade: ";
                cin >> id;
                submissionQueue.gradeSubmission(id);
                break;
                
            case 8:
                submissionQueue.displayStudentRankings();
                break;
                
            case 9:
                cout << "Logging out...\n";
                break;
                
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 9);
}

int main() {
    int choice;
    
    do {
        cout << "________________________________________\n";
        cout << "|     E-LEARNING ASSIGNMENT SYSTEM     |\n";
        cout << "|______________________________________|\n";
        cout << "| 1. Student Login                     |\n";
        cout << "| 2. Lecturer Login                    |\n";
        cout << "| 0. Exit System                       |\n";
        cout << "|______________________________________|\n";
        cout << "Choice: ";
        cin >> choice;
        system("cls");
        
        switch (choice) {
            case 1:
                studentMenu();
                break;
                
            case 2:
                lecturerMenu();
                break;
                
            case 0:
                cout << "\n=== Thank you for using the system! ===\n";
                break;
                
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    
    return 0;
}