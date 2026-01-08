//Implements (defines) everything declared in functions.h
//g++ main.cpp functions.cpp -o main


#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <windows.h>
#include "functions.h"
#include "semester.h"
#include "course.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

vector<Student> students;
int loggedInIndex = -1;


// GPA calculation
float calculateGPA(const vector<int>& marks) { //const will protect the marks from getting modified in the function
    int total = 0; 
    for (int m : marks)
        total += m;

    if (marks.empty()) return 0.0f; // if no marks are entered 

    return (total / (float)marks.size()) / 25;
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
    cin.ignore();          //just to clear (safety)
    getline(cin, s.name); //so that full name can be stored 

    cout << "Enter Roll Number: ";
    cin >> s.rollNo;

    cout << "Create Password: ";
    cin >> s.password;

    //Semester Setup
    Semester sem;
    cout << "\nEnter Semester Number: ";
    cin >> sem.semesterNumber;

    int courseCount;
    cout << "Enter Number of Courses: ";
    cin >> courseCount;

    sem.courses.clear(); //just for safety so that data may not fix in the vector
    sem.marks.clear();

    for (int i = 0; i < courseCount; i++) {
        Course c;
        cout << "\nEnter Course " << i + 1 << " Name: ";
        cin >> c.courseName; 

        c.totalSessions = 32;
        c.attendedSessions = 0;
        c.absentCount = 0;

        sem.courses.push_back(c); //the data will go to the course vector inside the samester

        int marks;
        cout << "Enter Marks for " << c.courseName << ": ";
        cin >> marks;
        sem.marks.push_back(marks); //the data will go to the marks vector inside the samester

    }

    sem.gpa = calculateGPA(sem.marks);

    // Student Setup

    s.semesters.push_back(sem); // now the entire semester info will be stored in the Semester vector inside the student struct along with name, pass and rollno.
    s.currentSemesterIndex = 0;

    s.isDropped = false;
    s.totalFee = 100000;
    s.paidFee = 0;

    students.push_back(s);
    saveToFile();

    SetConsoleTextAttribute(hConsole, 10);
    cout << "\nStudent registered successfully.\n";
    SetConsoleTextAttribute(hConsole, 7);

    pressEnterToContinue();
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

    cout << endl;
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
    cout << endl;
    SetConsoleTextAttribute(hConsole, 7);

    Student &s = students[loggedInIndex]; 
    Semester &sem = s.semesters[s.currentSemesterIndex]; 

    cout << "Name: " << s.name << endl;
    cout << "Roll Number: " << s.rollNo << endl;
    cout << "Semester: " << sem.semesterNumber << endl;

    cout << "\nCourses & Absences:\n";
    cout << endl;
    for (int i = 0; i < sem.courses.size(); i++) {
        cout << sem.courses[i].courseName
             << " Absents: "
             << sem.courses[i].absentCount << endl;
    }

    cout << "\nTotal Fee: " << s.totalFee << endl;
    cout << "Paid Fee: " << s.paidFee << endl;
    SetConsoleTextAttribute(hConsole, 10);
    cout << "Remaining Fee: " << (s.totalFee - s.paidFee) << endl;
    SetConsoleTextAttribute(hConsole, 7);
}



// View Marks and GPA
void viewMarks() {
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n------ Marks & GPA ------\n";
    cout << endl;
    SetConsoleTextAttribute(hConsole, 7);

    Semester &sem =students[loggedInIndex].semesters[students[loggedInIndex].currentSemesterIndex];

    for (int i = 0; i < sem.courses.size(); i++) {
        cout << sem.courses[i].courseName
             << ": " << sem.marks[i] << endl;
    }

    cout << "\nGPA: " << sem.gpa << endl;
}


// Mark Attendance (COURSE-WISE) — SEMESTER 
void markAttendance() {

    Student &s = students[loggedInIndex];
    Semester &sem = s.semesters[s.currentSemesterIndex];

    int courseChoice;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\nSelect Course:\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << endl;

    for (int i = 0; i < sem.courses.size(); i++) {
        cout << i + 1 << ". " << sem.courses[i].courseName << endl;
    }

    cout << "\nEnter choice: ";
    cin >> courseChoice;

    if (courseChoice < 1 || courseChoice > sem.courses.size()) {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\nInvalid course choice.\n";
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    Course &c = sem.courses[courseChoice - 1];

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

    if (sessionChoice == 1)
        sessionStartMinutes = 9 * 60;
    else if (sessionChoice == 2)
        sessionStartMinutes = 11 * 60;
    else {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\nInvalid session choice.\n";
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    bool present = false;

    // Attendance allowed within 1 hour after class starts
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
            cout << "\nWARNING: You have 6 absences in "
                 << c.courseName << ".\n";
            SetConsoleTextAttribute(hConsole, 7);
        }
        else if (c.absentCount == 7) {
            SetConsoleTextAttribute(hConsole, 14);
            cout << "\nWARNING: You have 7 absences in "
                 << c.courseName << ".\n";
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

//change semester
void changeSemester() {

    Student &s = students[loggedInIndex];

    int choice;

    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n------ Change Semester ------\n";
    SetConsoleTextAttribute(hConsole, 7);
    
    SetConsoleTextAttribute(hConsole, 10);
    cout << "\nExisting Semesters:\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << endl;

    for (int i = 0; i < s.semesters.size(); i++) {
        cout << i + 1 << ". Semester " << s.semesters[i].semesterNumber << endl;
    }

    SetConsoleTextAttribute(hConsole, 10);
    cout << "\n0. Create New Semester";
    SetConsoleTextAttribute(hConsole, 7);
    cout << endl;
    cout << "\nEnter choice: ";
    cin >> choice;

    // Switching semester
    if (choice >= 1 && choice <= s.semesters.size()) {
        s.currentSemesterIndex = choice - 1;

        SetConsoleTextAttribute(hConsole, 10);
        cout << "\nSemester switched successfully.\n";
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    // New Semester
    if (choice == 0) {

        Semester sem;

        cout << "\nEnter New Semester Number: ";
        cin >> sem.semesterNumber;

        int courseCount;
        cout << "Enter Number of Courses: ";
        cin >> courseCount;

        sem.courses.clear();
        sem.marks.clear();

        for (int i = 0; i < courseCount; i++) {

            Course c;
            cout << "\nEnter Course " << i + 1 << " Name: ";
            cin >> c.courseName;

            c.totalSessions = 32;
            c.attendedSessions = 0;
            c.absentCount = 0;

            sem.courses.push_back(c);

            int marks;
            cout << "Enter Marks for " << c.courseName << ": ";
            cin >> marks;
            sem.marks.push_back(marks);
        }

        sem.gpa = calculateGPA(sem.marks);

        s.semesters.push_back(sem);
        s.currentSemesterIndex = s.semesters.size() - 1;

        SetConsoleTextAttribute(hConsole, 10);
        cout << "\nNew semester created and activated.\n";
        SetConsoleTextAttribute(hConsole, 7);

        saveToFile();
        return;
    }

    SetConsoleTextAttribute(hConsole, 12);
    cout << "\nInvalid selection.\n";
    SetConsoleTextAttribute(hConsole, 7);
}


//Semester Report Generator
void viewSemesterReport() {

    Student &s = students[loggedInIndex];

    int choice;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n------ Semester Report ------\n";
    SetConsoleTextAttribute(hConsole, 7);

    cout << "\n1. Current Semester Report";
    cout << "\n2. All Semesters Report";
    cout << "\n3. Specific Semester Report"<< endl;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "\nEnter choice: ";
    SetConsoleTextAttribute(hConsole, 7);
    cin >> choice;

    // current semester
    if (choice == 1) {

        Semester &sem = s.semesters[s.currentSemesterIndex];
        SetConsoleTextAttribute(hConsole, 11);
        cout << "\n----------------------------------------\n";
        cout << "           Semester " << sem.semesterNumber << " Report\n";
        cout << "----------------------------------------\n";

        for (int i = 0; i < sem.courses.size(); i++) {
            Course &c = sem.courses[i];

            cout << "\nCourse: " << c.courseName << endl;
            SetConsoleTextAttribute(hConsole, 7);
            cout << "Marks: " << sem.marks[i] << endl;
            cout << "Attended Sessions: " << c.attendedSessions << endl;
            cout << "Absent Sessions:   " << c.absentCount << endl;

            SetConsoleTextAttribute(hConsole, 12);
            if (c.absentCount > 8)
                cout << "Status: DROPPED FROM COURSE\n";
            else
                SetConsoleTextAttribute(hConsole, 10);
                cout << "Status: ACTIVE\n";
                SetConsoleTextAttribute(hConsole, 11);
        }
        SetConsoleTextAttribute(hConsole, 10);
        cout << "\nGPA: " << sem.gpa << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }

    // For All semesters
    else if (choice == 2) {

        for (int sIndex = 0; sIndex < s.semesters.size(); sIndex++) {

            Semester &sem = s.semesters[sIndex];

            SetConsoleTextAttribute(hConsole, 11);
            cout << "\n----------------------------------------\n";
            cout << "          Semester " << sem.semesterNumber << " Report\n";
            cout << "----------------------------------------\n";
            SetConsoleTextAttribute(hConsole, 7);

            for (int i = 0; i < sem.courses.size(); i++) {
                Course &c = sem.courses[i];
                
                SetConsoleTextAttribute(hConsole, 11);
                cout << "\nCourse: " << c.courseName << endl;
                SetConsoleTextAttribute(hConsole, 7);
                cout << "Marks: " << sem.marks[i] << endl;
                cout << "Attended Sessions: " << c.attendedSessions << endl;
                cout << "Absent Sessions:   " << c.absentCount << endl;

                SetConsoleTextAttribute(hConsole, 12);
                if (c.absentCount > 8)
                    cout << "Status: DROPPED FROM COURSE\n";
                else if (c.absentCount < 9);
                SetConsoleTextAttribute(hConsole, 10);
                    cout << "Status: ACTIVE\n";
                    SetConsoleTextAttribute(hConsole, 7);
            }
             
            SetConsoleTextAttribute(hConsole, 10);
            cout << "\nGPA: " << sem.gpa << endl;
            SetConsoleTextAttribute(hConsole, 7);
        }
    }

    // For a particular Semester 
    else if (choice == 3) {

        int semChoice;
        SetConsoleTextAttribute(hConsole, 10);
        cout << "\nAvailable Semesters:\n";
        cout << endl;
        SetConsoleTextAttribute(hConsole, 7);
        for (int i = 0; i < s.semesters.size(); i++) {
            cout << i + 1 << ". Semester " << s.semesters[i].semesterNumber << endl;
        }

        SetConsoleTextAttribute(hConsole, 11);
        cout << "\nEnter semester number: ";
        SetConsoleTextAttribute(hConsole, 7);
        cin >> semChoice;

        if (semChoice < 1 || semChoice > s.semesters.size()) {
            SetConsoleTextAttribute(hConsole, 12);
            cout << "\nInvalid semester selection.\n";
            SetConsoleTextAttribute(hConsole, 7);
            return;
        }

        Semester &sem = s.semesters[semChoice - 1];

        SetConsoleTextAttribute(hConsole, 11);
        cout << "\n----------------------------------------\n";
        cout << "          Semester " << sem.semesterNumber << " Report\n";
        cout << "----------------------------------------\n";
        SetConsoleTextAttribute(hConsole, 7);

        for (int i = 0; i < sem.courses.size(); i++) {
            Course &c = sem.courses[i];

            SetConsoleTextAttribute(hConsole, 11);
            cout << "\nCourse: " << c.courseName << endl;
            SetConsoleTextAttribute(hConsole, 7);
            cout << "Marks: " << sem.marks[i] << endl;
            cout << "Attended Sessions: " << c.attendedSessions << endl;
            cout << "Absent Sessions:   " << c.absentCount << endl;

            SetConsoleTextAttribute(hConsole, 12);
            if (c.absentCount > 8)
                cout << "Status: DROPPED FROM COURSE\n";
            else
            SetConsoleTextAttribute(hConsole, 10);
                cout << "Status: ACTIVE\n";
                SetConsoleTextAttribute(hConsole, 7);
        }

        SetConsoleTextAttribute(hConsole, 10);
        cout << "\nGPA: " << sem.gpa << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }

    else {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\nInvalid choice.\n";
        SetConsoleTextAttribute(hConsole, 7);
    }
}

// View Timetable (with attendace timing)
void viewTimetable() {

    SetConsoleTextAttribute(hConsole, 11);
    cout << "\n------ Class Timetable ------\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << endl;

    Student &s = students[loggedInIndex];
    Semester &sem = s.semesters[s.currentSemesterIndex];

    if (sem.courses.empty()) {
        cout << "No courses registered for this semester.\n";
        return;
    }

    SetConsoleTextAttribute(hConsole, 10);
    cout <<           "Semester " << sem.semesterNumber << " Timetable\n";
    SetConsoleTextAttribute(hConsole, 7);
    cout << "----------------------------------------\n";

    SetConsoleTextAttribute(hConsole, 10);
    cout << "\nSession 1 (9:00 AM):\n";
    SetConsoleTextAttribute(hConsole, 7);
    for (int i = 0; i < sem.courses.size(); i++) {
        cout << sem.courses[i].courseName << endl;
    }
    SetConsoleTextAttribute(hConsole, 10);
    cout << "\nSession 2 (11:00 AM):\n";
    SetConsoleTextAttribute(hConsole, 7);
    for (int i = 0; i < sem.courses.size(); i++) {
        cout << sem.courses[i].courseName << endl;
    }
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

        Student &s = students[loggedInIndex];

        SetConsoleTextAttribute(hConsole, 10);
if (s.semesters.empty()) {
    cout << "Active Semester: NOT SET\n"; //to prevent crash that happen
} else {
    cout << "Active Semester: "
         << s.semesters[s.currentSemesterIndex].semesterNumber
         << endl;
        SetConsoleTextAttribute(hConsole, 11);
        cout << "----------------------------------------\n";
        SetConsoleTextAttribute(hConsole, 7);
}
        cout << "\n1. View Profile";
        cout << "\n2. View Marks and GPA";
        cout << "\n3. Mark Attendance";
        cout << "\n4. View Timetable";
        cout << "\n5. Fee Payment";
        cout << "\n6. Change Semester";
        cout << "\n7. View Semester Report";
        cout << "\n8. Logout\n";

        SetConsoleTextAttribute(hConsole, 11);
        cout << "\nEnter your choice: ";
        SetConsoleTextAttribute(hConsole, 7);
        cin >> choice;

        switch (choice) {
            case 1: viewProfile(); pressEnterToContinue(); break;
            case 2: viewMarks(); pressEnterToContinue(); break;
            case 3: markAttendance(); saveToFile(); pressEnterToContinue(); break;
            case 4: viewTimetable(); pressEnterToContinue(); break;
            case 5: feePayment(); saveToFile(); pressEnterToContinue(); break;
            case 6: changeSemester(); saveToFile(); pressEnterToContinue(); break;
            case 7: viewSemesterReport();saveToFile(); pressEnterToContinue(); break;
            case 8:
                saveToFile();
                loggedInIndex = -1;
                break;

            default:
                cout << "\nInvalid choice. Please try again.\n";
                pressEnterToContinue();
        }
    } while (choice != 8);
}


// File Handling
void saveToFile() {
    ofstream file("data.txt");

    file << students.size() << endl;  // number of students for file >> studentCount; in loadFromfile()

    for (Student &s : students) {
        file << s.rollNo << endl;
        file << s.password << endl;
        file << s.name << endl;

        file << s.semesters.size() << endl;      // number of semesters for file >> semesterCount; in loadFromfile()
        file << s.currentSemesterIndex << endl;

        // Save all semesters
        for (Semester &sem : s.semesters) {
            file << sem.semesterNumber << endl;
            file << sem.courses.size() << endl;  // number of course for file >> course; in loadFromfile()

            // Save courses
            for (Course &c : sem.courses) {
                file << c.courseName << endl;
                file << c.totalSessions << endl;
                file << c.attendedSessions << endl;
                file << c.absentCount << endl;
            }

            // Save marks
            file << sem.marks.size() << endl;
            for (int m : sem.marks)
                file << m << endl;

            file << sem.gpa << endl;
        }

        file << s.totalFee << endl;
        file << s.paidFee << endl;
        file << s.isDropped << endl;
    }

    file.close();
}


void loadFromFile() {
    ifstream file("data.txt");
    if (!file) return;

    students.clear();

    int studentCount;
    file >> studentCount;

    for (int i = 0; i < studentCount; i++) {
        Student s;

        file >> s.rollNo;
        file >> s.password;
        file.ignore();
        getline(file, s.name);

        int semesterCount;
        file >> semesterCount;
        file >> s.currentSemesterIndex;

        s.semesters.clear();

        for (int j = 0; j < semesterCount; j++) {
            Semester sem;

            file >> sem.semesterNumber;

            int courseCount;
            file >> courseCount;

            sem.courses.clear();
            for (int k = 0; k < courseCount; k++) {
                Course c;
                file.ignore();
                getline(file, c.courseName);
                file >> c.totalSessions;
                file >> c.attendedSessions;
                file >> c.absentCount;
                sem.courses.push_back(c);
            }

            int marksCount;
            file >> marksCount;

            sem.marks.clear();
            for (int m = 0; m < marksCount; m++) {
                int mark;
                file >> mark;
                sem.marks.push_back(mark);
            }

            file >> sem.gpa;

            s.semesters.push_back(sem);
        }

        file >> s.totalFee;
        file >> s.paidFee;
        file >> s.isDropped;

        students.push_back(s);
    }

    file.close();
}


void pressEnterToContinue() {
    SetConsoleTextAttribute(hConsole, 3);
    cout << "\nEnter X to Return to Menu: ";
    SetConsoleTextAttribute(hConsole, 7);
    cin.ignore();
    cin.get();
}

