#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

// Constants for file names and other configurations
const string PASSWORD_FILE = "password.txt";
const string STUDENT_FILE = "students.txt";
const char XOR_KEY = 'K';               // Key used for simple XOR encryption
const int MAX_STUDENTS = 100;           // Maximum number of students that can be managed

// Class to represent a student
class Student {
public:
    int r, s, yr;             // Roll number, semester, and year
    long long c;              // Contact number
    string na, ad, f;         // Name, address, and faculty

    // Overloading the output stream operator to save student details to a file
    friend ofstream& operator<<(ofstream& os, const Student& std) {
        os << std.na << "\n" 
           << std.ad << "\n" 
           << std.r << "\n" 
           << std.c << "\n" 
           << std.f << "\n" 
           << std.s << "\n" 
           << std.yr << "\n";
        return os;
    }

    // Overloading the input stream operator to load student details from a file
    friend ifstream& operator>>(ifstream& is, Student& std) {
        getline(is, std.na);
        getline(is, std.ad);
        is >> std.r >> std.c >> std.f >> std.s >> std.yr;
        is.ignore();   // Ignore the newline character after the year
        return is;
    }

    // Method to display student details in a formatted manner
    void display(int index) const {
        cout << left << setw(8) << index
             << left << setw(16) << na
             << left << setw(16) << ad
             << left << setw(16) << r
             << left << setw(16) << c
             << left << setw(10) << f
             << left << setw(10) << s
             << left << setw(10) << yr
             << "\n";
    }
};

// Class to manage the student records
class StudentManager {
public:
    // Load student records from the file
    bool loadStudents(Student students[], int& count) {
        ifstream file(STUDENT_FILE);
        if (!file.is_open()) {
            cout << "Error opening file for reading.\n";
            return false;
        }

        count = 0;
        while (file >> students[count] && count < MAX_STUDENTS) {
            count++;
        }

        file.close();
        return true;
    }

    // Save all student records to the file
    bool saveStudents(const Student students[], int count) {
        ofstream file(STUDENT_FILE, ios::trunc);
        if (!file.is_open()) {
            cout << "Error opening file for writing.\n";
            return false;
        }

        for (int i = 0; i < count; ++i) {
            file << students[i];
        }

        file.close();
        return true;
    }

    // Add a single student record to the file
    void addStudent(const Student& student) {
        ofstream file(STUDENT_FILE, ios::app);
        if (!file.is_open()) {
            cout << "Error opening file for writing.\n";
            return;
        }

        file << student;
        file.close();
    }
};

// Utility function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");    // For Windows
#else
    system("clear");  // For Unix/Linux/MacOS
#endif
}

// Function to display the home page with system information
void displayHomePage() {
    clearScreen();
    cout << setw(40) << "" << "WELCOME TO STUDENT DATA MANAGEMENT SYSTEM" << endl;
    cout << setw(42) << "" << "Object Oriented Programming [CT 501]" << endl;
    cout << setw(43) << "" << "BCT II/I IOE-PASHCHIMANCHAL CAMPUS" << endl << endl;
    cout << setw(38) << "" << "Developed by: Aabiskar Regmi PAS079BCT001" << endl<<endl;
}

// Function to input new student data and add it to the file
void inp(StudentManager& manager, Student students[], int& count) {
    cout << "Enter student data (enter 'q' to stop):\n";
    while (count < MAX_STUDENTS) {
        Student student;
        string input;
        cout << "Enter name (or 'q' to quit): ";
        getline(cin, input);
        if (input == "q" || input == "Q") break;

        student.na = input;

        cout << "Enter address: ";
        getline(cin, student.ad);

        cout << "Enter roll no: ";
        cin >> student.r;

        cout << "Enter contact no: ";
        cin >> student.c;

        cout << "Enter faculty: ";
        cin >> student.f;

        cout << "Enter semester: ";
        cin >> student.s;
        cin.ignore();  // Ignore the newline character after semester

        cout << "Enter year: ";
        cin >> student.yr;
        cin.ignore();  // Ignore the newline character after year

        students[count++] = student;
        manager.addStudent(student);  // Save the new student to the file
    }
}

// Function to display all student records
void disstd(const Student students[], int count) {
    clearScreen();

    // Print table headers
    cout << left << setw(8) << "S.N."
         << left << setw(16) << "Name"
         << left << setw(16) << "Address"
         << left << setw(16) << "Roll No."
         << left << setw(16) << "Contact"
         << left << setw(10) << "Faculty"
         << left << setw(10) << "Semester"
         << left << setw(10) << "Year"
         << "\n";

    cout << string(96, '-') << "\n";  // Separator line

    // Display each student record
    for (int i = 0; i < count; ++i) {
        students[i].display(i + 1);
    }
}

// Function to add a new student record
void addstd(StudentManager& manager, Student students[], int& count) {
    clearScreen();
    inp(manager, students, count);  // Get new student details and save them
    cout << "Data added successfully!\n";
}

// Function to sort student records based on a selected criterion
void sortstd(Student students[], int count) {
    int choice;
    clearScreen();

    // Display sorting options
    cout << "Sort by:\n1. Name\n2. Roll Number\n3. Year\nEnter your choice: ";
    cin >> choice;

    // Sort based on the user's choice
    switch(choice) {
        case 1:
            sort(students, students + count, [](const Student& a, const Student& b) { return a.na < b.na; });
            cout << "Data sorted by Name.\n";
            break;

        case 2:
            sort(students, students + count, [](const Student& a, const Student& b) { return a.r < b.r; });
            cout << "Data sorted by Roll Number.\n";
            break;
        case 3:
            sort(students, students + count, [](const Student& a, const Student& b) { return a.yr < b.yr; });
            cout << "Data sorted by Year.\n";
            break;
        default:
            cout << "Invalid choice. Sorting cancelled.\n";
            return;
    }

    cout << "Data sorted successfully!\n";
    cout << "Displaying sorted data:\n";
    disstd(students, count);  // Display sorted records
}

// Function to search for a student by name
void searchstd(const Student students[], int count) {
    string searchTerm;
    clearScreen();

    cout << "Enter name to search: ";
    cin.ignore();  // Ignore the newline character before getting the search term
    getline(cin, searchTerm);

    bool found = false;
    // Loop through each student and check if the name matches the search term
    for (int i = 0; i < count; ++i) {
        if (students[i].na.find(searchTerm) != string::npos) {
            if (!found) {
                // Print table headers only if at least one match is found
                cout << "\nSearch Results:\n";
                cout << left << setw(8) << "S.N."
                     << left << setw(16) << "Name"
                     << left << setw(16) << "Address"
                     << left << setw(16) << "Roll No."
                     << left << setw(16) << "Contact"
                     << left << setw(10) << "Faculty"
                     << left << setw(10) << "Semester"
                     << left << setw(10) << "Year"
                     << "\n";
                cout << string(96, '-') << "\n";  // Separator line
            }
            students[i].display(i + 1);
            found = true;
        }
    }

    if (!found) {
        cout << "No matching records found.\n";
    }
}

// Function to delete student data by name
void deletestd(StudentManager& manager, Student students[], int& count) {
    string nameToDelete;
    clearScreen();

    cout << "Enter name to delete: ";
    getline(cin, nameToDelete);

    // Track matching records
    int matchCount = 0;
    int matchingIndices[MAX_STUDENTS];

    // Find all matching records
    for (int i = 0; i < count; ++i) {
        if (students[i].na == nameToDelete) {
            matchingIndices[matchCount++] = i;
        }
    }

    // If no match is found
    if (matchCount == 0) {
        cout << "No student found with the name \"" << nameToDelete << "\".\n";
        return;
    }

    // If multiple matches are found
    if (matchCount > 1) {
        cout << "Multiple students found with the name \"" << nameToDelete << "\":\n";
        for (int i = 0; i < matchCount; ++i) {
            int index = matchingIndices[i];
            cout << i + 1 << ". ";
            students[index].display(index + 1);
        }

        // Ask user to choose which student to delete
        int choice;
        cout << "Enter the number of the student you want to delete: ";
        cin >> choice;

        if (choice < 1 || choice > matchCount) {
            cout << "Invalid choice. Deletion cancelled.\n";
            return;
        }

        // Remove the selected student
        int indexToDelete = matchingIndices[choice - 1];
        for (int i = indexToDelete; i < count - 1; ++i) {
            students[i] = students[i + 1];
        }
        count--;
        manager.saveStudents(students, count);
        cout << "Student record deleted successfully!\n";
    } else {
        // If only one match is found, delete it directly
        int indexToDelete = matchingIndices[0];
        for (int i = indexToDelete; i < count - 1; ++i) {
            students[i] = students[i + 1];
        }
        count--;
        manager.saveStudents(students, count);
        cout << "Student record deleted successfully!\n";
    }
}

// Function to display the main menu and get user choice
int menu() {
    int choice;
    cout << "Choose what you want to do\n"
         << "1. Display existing data\n"
         << "2. Add new data\n"
         << "3. Sort data\n"
         << "4. Search data\n"
         << "5. Delete data\n"
         << "6. Change login credentials\n"
         << "7. Exit\n";
    cin >> choice;
    cin.ignore(); // Ignore newline character after choice
    return choice;
}

// Function to encrypt or decrypt a string using XOR
string encryptDecrypt(const string& toEncrypt) {
    string output = toEncrypt;
    for (int i = 0; i < toEncrypt.size(); i++)
        output[i] = toEncrypt[i] ^ XOR_KEY;
    return output;
}

// Function to save encrypted credentials to a file
void saveCredentials(const string& username, const string& password) {
    ofstream file(PASSWORD_FILE);
    if (file.is_open()) {
        file << encryptDecrypt(username) << endl;
        file << encryptDecrypt(password);
        file.close();
        cout << "Credentials updated successfully!\n";
    } else {
        cout << "Unable to save credentials.\n";
    }
}

// Function to load and decrypt credentials from a file
bool loadCredentials(string& username, string& password) {
    ifstream file(PASSWORD_FILE);
    if (file.is_open()) {
        getline(file, username);
        getline(file, password);
        file.close();
        username = encryptDecrypt(username);
        password = encryptDecrypt(password);
        return true;
    }
    return false;
}

// Function to initialize necessary files if they don't exist
void initializeFiles() {
    // Initialize password file if it doesn't exist
    ifstream passFile(PASSWORD_FILE);
    if (!passFile.good()) {
        ofstream newPassFile(PASSWORD_FILE);
        if (newPassFile.is_open()) {
            newPassFile << encryptDecrypt("admin") << endl;
            newPassFile << encryptDecrypt("admin");
            newPassFile.close();
        }
    }
    passFile.close();

    // Initialize student file if it doesn't exist
    ifstream studFile(STUDENT_FILE);
    if (!studFile.good()) {
        ofstream newStudFile(STUDENT_FILE);
        newStudFile.close();
    }
    studFile.close();
}

// Function to handle user login
bool login() {
    string storedUsername, storedPassword;
    if (!loadCredentials(storedUsername, storedPassword)) {
        cout << "Error loading credentials. Using default.\n";
        storedUsername = "admin";
        storedPassword = "admin";
    }

    string username, password;
    cout << setw(36) << "" << "Enter username: ";
    cin >> username;
    cout << setw(36) << "" << "Enter password: ";
    cin >> password;

    if (username == storedUsername && password == storedPassword) {
        cout << setw(36) << "" << "Login successful!" << endl;
        return true;
    } else {
        cout << setw(34) << "" << "Invalid credentials!" << endl;
        return false;
    }
}

// Function to change the login credentials
void changeCredentials() {
    clearScreen();
    string newUsername, newPassword;
    cout << "Enter new username: ";
    cin >> newUsername;
    cout << "Enter new password: ";
    cin >> newPassword;
    saveCredentials(newUsername, newPassword);
}

int main() {
    initializeFiles(); // Ensure necessary files are created
    displayHomePage(); // Display welcome screen

    if (!login()) { // Perform login
        cout << "Exiting...\n";
        return 1;
    }

    StudentManager manager; // Manage student data
    Student students[MAX_STUDENTS]; // Array to store students
    int studentCount = 0; // Track number of students

    while (true) {
        clearScreen(); // Clear the console
        int choice = menu(); // Display menu and get user choice

        switch (choice) {
            case 1:
                if (manager.loadStudents(students, studentCount)) {
                    disstd(students, studentCount); // Display student data
                } else {
                    cout << "Failed to load students.\n";
                }
                break;
            case 2:
                addstd(manager, students, studentCount); // Add new student
                break;
            case 3:
                sortstd(students, studentCount); // Sort student data
                manager.saveStudents(students, studentCount); // Save sorted data
                break;
            case 4:
                searchstd(students, studentCount); // Search student data
                break;
            case 5:
                deletestd(manager, students, studentCount); // Delete student data
                break;
            case 6:
                changeCredentials(); // Change login credentials
                break;
            case 7:
                clearScreen();
                cout << "Exiting...\n";
                return 0; // Exit program
            default:
                cout << "Invalid choice! Please try again.\n";
        }
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get(); // Wait for user to press Enter
    }

    return 0;
}

