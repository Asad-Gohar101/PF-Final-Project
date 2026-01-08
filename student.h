/*Only the Student structure (struct) is defined in this header file.
 It tells the compiler what a Student is. it exists so that other files can include it and use the Student structure.*/  

#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

// Forward declaration as we have used it in sStudent as well
struct Semester;

using namespace std;

struct Student {
    string rollNo;
    string password;
    string name;

    vector<Semester> semesters;
    int currentSemesterIndex;

    bool isDropped;
    int totalFee;
    int paidFee;
};

#endif
