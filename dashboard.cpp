
#include <iostream>
#include <string>
#include "dashboard.h"
#include "deadline.h"
#include "todo.h"
#include "notes.h"

using namespace std;

void dashboard(const string &studentName)
{
    int choice;

    do
    {
        cout << "\n========================================\n";
        cout << "           STUDENT DASHBOARD\n";
        cout << "========================================\n";
        cout << "Hello, " << studentName << "!\n\n";

        cout << "1. Add Deadline\n";
        cout << "2. Add Todo\n";
        cout << "3. Add Notes\n";
        cout << "4. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        cin.ignore(10000, '\n');

        switch (choice)
        {
        case 1:
            deadlineMenu();
            break;

        case 2:
            todoMenu();
            break;

        case 3:
            notesMenu();
            break;

        case 4:
            cout << "\nExiting dashboard...\n";
            break;

        default:
            cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 4);
}
