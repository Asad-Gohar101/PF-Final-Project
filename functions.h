//Functions (and global variables) are declared here, to be defined in functions.cpp

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include "student.h"

using namespace std;

extern vector<Student> students;  //extern tells the compiler that the variable is defined elsewhere
extern int loggedInIndex;

float calculateGPA(int marks[]);
void registerStudent();
bool loginStudent();
void studentDashboard();
void viewProfile();
void viewMarks();
void markAttendance();
void feePayment();
void viewTimetable();
void pressEnterToContinue();

void saveToFile();
void loadFromFile();

#endif
