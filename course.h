#ifndef COURSE_H
#define COURSE_H

#include <string>
using namespace std;

struct Course {
    string courseName;
    int totalSessions;
    int attendedSessions;
    int absentCount;
};

#endif
