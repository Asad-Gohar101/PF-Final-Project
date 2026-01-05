//Implements (defines) everything declared in functions.h
//g++ main.cpp functions.cpp -o main


#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <windows.h>
#include "functions.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

vector<Student> students;
int loggedInIndex = -1;


// GPA calculation
float calculateGPA(int marks[]) {
    int total = 0;
    for (int i = 0; i < 3; i++)
        total += marks[i];
    return (total / 3.0) / 25;
}


// Registration
void registerStudent() {
    Student s;
    cout << endl;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n          Student Registration\n";
    cout << "----------------------------------------\n";
    SetConsoleTextAttribute(hConsole, 7);

    cout << "Enter Name: ";
    cin >> s.name;
    cout << "Enter Roll Number: ";
    cin >> s.rollNo;
    cout << "Create Password: ";
    cin >> s.password;

    cout << "Maths Marks: "; cin >> s.marks[0];
    cout << "Physics Marks: "; cin >> s.marks[1];
    cout << "Programming Marks: "; cin >> s.marks[2];

    s.gpa = calculateGPA(s.marks);

    s.courses[0].courseName = "Maths";
    s.courses[0].totalSessions = 32;
    s.courses[0].attendedSessions = 0;
    s.courses[0].absentCount = 0;

    s.courses[1].courseName = "Physics";
    s.courses[1].totalSessions = 32;
    s.courses[1].attendedSessions = 0;
    s.courses[1].absentCount = 0;

    s.courses[2].courseName = "Programming";
    s.courses[2].totalSessions = 32;
    s.courses[2].attendedSessions = 0;
    s.courses[2].absentCount = 0;

    s.isDropped = false;
    s.totalFee = 100000;
    s.paidFee = 0;

    students.push_back(s);
    saveToFile();

SetConsoleTextAttribute(hConsole, 10);
cout << "\nStudent registered successfully.\n";
pressEnterToContinue();
SetConsoleTextAttribute(hConsole, 7);

}


// Login
bool loginStudent() {
    string roll, pass;
    cout << endl;

    SetConsoleTextAttribute(hConsole, 11);
    cout << "            Student Login\n";
    cout << "----------------------------------------\n";
    SetConsoleTextAttribute(hConsole, 7);

    cout << "\nEnter Roll Number: ";
    cin >> roll;
    cout << "Enter Password: ";
    cin >> pass;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].rollNo == roll && students[i].password == pass) {
            loggedInIndex = i;
            SetConsoleTextAttribute(hConsole, 10);
            cout << "\nLogin successful.\n";
            cout << "\nExter Y To Open Student Portal: ";
            cin.ignore();
            cin.get();
            SetConsoleTextAttribute(hConsole, 7);
            return true;
        }
    }

    SetConsoleTextAttribute(hConsole, 12);
    cout << "\nInvalid credentials.\n";
    pressEnterToContinue();
    SetConsoleTextAttribute(hConsole, 7);
    return false;
}


// Fee Payment
void feePayment() {
    int amount;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n------ Fee Payment ------\n";
    SetConsoleTextAttribute(hConsole, 7);

    cout << "\nTotal Fee: " << students[loggedInIndex].totalFee;
    cout << "\nPaid Fee: " << students[loggedInIndex].paidFee;
    cout << "\nRemaining Fee: " 
         << (students[loggedInIndex].totalFee - students[loggedInIndex].paidFee);

    if (students[loggedInIndex].paidFee >= students[loggedInIndex].totalFee) {
        cout << "\nAll fees are already paid.\n";
        return;
    }

    cout << "\nEnter amount to pay: ";
    cin >> amount;

    if (students[loggedInIndex].paidFee + amount >
        students[loggedInIndex].totalFee) {

        SetConsoleTextAttribute(hConsole, 12);
        cout << "Payment exceeds total fee.\n";
        SetConsoleTextAttribute(hConsole, 7);

    } else {
        students[loggedInIndex].paidFee += amount;
        SetConsoleTextAttribute(hConsole, 10);
        cout << "\nPayment successful.\n";
         cout << "\nRemaining Fee: " 
         << (students[loggedInIndex].totalFee - students[loggedInIndex].paidFee) <<endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
}


// View profile
void viewProfile() {
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n------ Student Profile ------\n";
    SetConsoleTextAttribute(hConsole, 7);

    cout << "Name: " << students[loggedInIndex].name << endl;
    cout << "Roll Number: " << students[loggedInIndex].rollNo << endl;

    for (int i = 0; i < 3; i++) {
        cout << students[loggedInIndex].courses[i].courseName
             << " Absents: "
             << students[loggedInIndex].courses[i].absentCount << endl;
    }

    cout << "Total Fee: " << students[loggedInIndex].totalFee << endl;
    cout << "Paid Fee: " << students[loggedInIndex].paidFee << endl;
    cout << "Remaining Fee: " << (students[loggedInIndex].totalFee - students[loggedInIndex].paidFee);
    cout << endl;
}


// View Marks and GPA
void viewMarks() {
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n------ Marks & GPA ------\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "Maths: " << students[loggedInIndex].marks[0] << endl;
    cout << "Physics: " << students[loggedInIndex].marks[1] << endl;
    cout << "Programming: " << students[loggedInIndex].marks[2] << endl;
    cout << "GPA: " << students[loggedInIndex].gpa << endl;
}


// Mark Attendance (COURSE-WISE)
void markAttendance() {

    int courseChoice;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\nSelect Course:\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << endl;
    cout << "1. Maths\n2. Physics\n3. Programming\n";
    cout << "\nEnter choice: ";
    cin >> courseChoice;

    if (courseChoice < 1 || courseChoice > 3) {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\nInvalid course choice.\n";
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    Course &c = students[loggedInIndex].courses[courseChoice - 1];

    if (c.absentCount > 8) {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\nYou are dropped from " << c.courseName << ".\n";
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    int sessionChoice;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\nSelect Session:\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << endl;
    cout << "1. Session (9:00 AM)\n";
    cout << "2. Session (11:00 AM)\n";
    cout << "\nEnter choice: ";
    cin >> sessionChoice;

    time_t now = time(0);
    tm* localTime = localtime(&now);

    int currentMinutes = localTime->tm_hour * 60 + localTime->tm_min;

    int sessionStartMinutes;

    if (sessionChoice == 1) {
        sessionStartMinutes = 9 * 60;    // 9:00 AM
    }
    else if (sessionChoice == 2) {
        sessionStartMinutes = 11 * 60;   // 11:00 AM
    }
    else {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\nInvalid session choice.\n";
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    bool present = false;

    // Attendance allowed only within 1 hour AFTER class starts
    if (currentMinutes >= sessionStartMinutes &&
        currentMinutes <= sessionStartMinutes + 60) {
        present = true;
    }

    if (present) {
        SetConsoleTextAttribute(hConsole, 10);
        cout << "\nAttendance Marked: PRESENT\n";
        SetConsoleTextAttribute(hConsole, 7);
        c.attendedSessions++;
    }
    else {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\nAttendance Window Missed. Marked ABSENT\n";
        SetConsoleTextAttribute(hConsole, 7);
        c.absentCount++;

        if (c.absentCount == 6) {
            SetConsoleTextAttribute(hConsole, 14);
            cout << "\nWARNING: You have 6 absences in " << c.courseName
                 << ". Attendance is critical.\n";
            SetConsoleTextAttribute(hConsole, 7);
        }
        else if (c.absentCount == 7) {
            SetConsoleTextAttribute(hConsole, 14);
            cout << "\nWARNING: You have 7 absences in " << c.courseName
                 << ". Attendance is critical.\n";
            SetConsoleTextAttribute(hConsole, 7);
        }
        else if (c.absentCount == 8) {
            SetConsoleTextAttribute(hConsole, 12);
            cout << "\nFINAL WARNING: One more absence will drop you from "
                 << c.courseName << ".\n";
            SetConsoleTextAttribute(hConsole, 7);
        }
    }

    if (c.absentCount > 8) {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\nSTATUS: DROPPED FROM COURSE -> "
             << c.courseName << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }

    int completed = c.attendedSessions + c.absentCount;
    int remaining = c.totalSessions - completed;

    cout << "\nAttendance Summary:\n";
    cout << "Attended Sessions: " << c.attendedSessions << endl;
    cout << "Absent Sessions: " << c.absentCount << endl;
    cout << "Remaining Classes: " << remaining << endl;
}



// View Timetable
void viewTimetable() {
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n------ Class Timetable ------\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout <<endl;
    cout << "Monday    : Maths (9:00 - 10:30), Programming (11:00 - 12:30)\n";
    cout << "Tuesday   : Physics (9:00 - 10:30), Maths (11:00 - 12:30)\n";
    cout << "Wednesday : Programming (9:00 - 10:30), Physics (11:00 - 12:30)\n";
    cout << "Thursday  : Maths (9:00 - 10:30), Programming (11:00 - 12:30)\n";
    cout << "Friday    : Physics (9:00 - 10:30)\n";
}


// Dashboard
void studentDashboard() {
    int choice;
    do {
        system("cls");

        cout << endl;
        SetConsoleTextAttribute(hConsole, 11);
        cout << "----------------------------------------\n";
        cout << "           Student Dashboard\n";
        cout << endl;
        SetConsoleTextAttribute(hConsole, 10);
        cout << "Welcome: " << students[loggedInIndex].name
             << " (Roll No: " << students[loggedInIndex].rollNo << ")\n";
        SetConsoleTextAttribute(hConsole, 11);
        cout << "----------------------------------------\n";
        SetConsoleTextAttribute(hConsole, 7);

        cout << "\n1. View Profile\n2. View Marks and GPA\n3. Mark Attendance\n4. View Timetable\n5. Fee Payment\n6. Logout\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewProfile(); pressEnterToContinue(); break;
            case 2: viewMarks(); pressEnterToContinue(); break;
            case 3: markAttendance(); pressEnterToContinue(); break;
            case 4: viewTimetable(); pressEnterToContinue(); break;
            case 5: feePayment(); pressEnterToContinue(); break;
            case 6:
                saveToFile();
                loggedInIndex = -1;
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
                pressEnterToContinue();
        }
    } while (choice != 6);
}


// File Handling
void saveToFile() {
    ofstream file("data.txt");

    for (int i = 0; i < students.size(); i++) {
        file << students[i].rollNo << endl;
        file << students[i].password << endl;
        file << students[i].name << endl;

        for (int j = 0; j < 3; j++)
            file << students[i].marks[j] << " ";
        file << endl;

        file << students[i].gpa << endl;

        for (int c = 0; c < 3; c++) {
            file << students[i].courses[c].attendedSessions << endl;
            file << students[i].courses[c].absentCount << endl;
        }

        file << students[i].totalFee << endl;
        file << students[i].paidFee << endl;
    }

    file.close();
}



void loadFromFile() {
    ifstream file("data.txt");
    if (!file) return;

    students.clear();   // IMPORTANT

    while (true) {
        Student s;

        if (!(file >> s.rollNo))
            break;

        file >> s.password;
        file.ignore();
        getline(file, s.name);

        for (int i = 0; i < 3; i++)
            file >> s.marks[i];

        file >> s.gpa;

        for (int c = 0; c < 3; c++) {
            file >> s.courses[c].attendedSessions;
            file >> s.courses[c].absentCount;
        }

        // MUST read these (they were written!)
        file >> s.totalFee;
        file >> s.paidFee;

        // Restore course metadata
        s.courses[0].courseName = "Maths";
        s.courses[1].courseName = "Physics";
        s.courses[2].courseName = "Programming";

        for (int c = 0; c < 3; c++)
            s.courses[c].totalSessions = 32;

        students.push_back(s);
    }

    file.close();
}

void pressEnterToContinue() {
    SetConsoleTextAttribute(hConsole, 3);
    cout << "\nExter x Return to Menu: ";
    SetConsoleTextAttribute(hConsole, 7);
    cin.ignore();
    cin.get();
}
