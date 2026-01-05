/*Only the Student structure (struct) is defined in this header file.
 It tells the compiler what a Student is. it exists so that other files can include it and use the Student structure.*/  

#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include "course.h"

using namespace std;

struct Student {
    string rollNo;
    string password;
    string name;
    int marks[3];

    Course courses[3];
    
    float gpa;
    bool isDropped;

    int totalFee;
    int paidFee;
};

#endif
