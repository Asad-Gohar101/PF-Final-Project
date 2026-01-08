#ifndef SEMESTER_H
#define SEMESTER_H

#include <vector>
#include "course.h"

using namespace std;

struct Semester {
    int semesterNumber;
    vector<Course> courses;
    vector<int> marks;
    float gpa;
};

#endif
