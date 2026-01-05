//countains the main() function. It uses functions defined in functions.cpp and the Student structure defined in student.h (included in functions.h) to control the flow of the program.

/*
1️. main.cpp starts execution
2️. It includes functions.h
3️. functions.h includes student.h
4️. Compiler now knows:

What a Student is
What functions exist

5️. When functions are called, their definitions are found in functions.cpp*/

#include <iostream>
#include <windows.h>
#include "functions.h"

using namespace std;


int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    loadFromFile();

    int choice;
    do {
        system("cls"); 
        
        SetConsoleTextAttribute(hConsole, 11);
        cout << "========================================\n";
        cout << "           Student Portal\n";
        cout << "========================================\n";
        SetConsoleTextAttribute(hConsole, 7);

        cout << "\n1. Register\n2. Login\n3. Exit\n";

        cout<<"\nEnter your choice: ";

        cin >> choice;

        if (choice == 1)
            registerStudent();
        else if (choice == 2 && loginStudent())
            studentDashboard();
        else if (choice == 3)
            saveToFile();
        else
            cout << "\nInvalid choice. Please try again.\n";

    } while (choice != 3);

    return 0;
}
