
#include <iostream>
#include <string>
#include "dashboard.h"

using namespace std;

int main()
{
    string studentName;

    cout << "========================================\n";
    cout << "        WELCOME TO STUDYSPHERE\n";
    cout << "========================================\n\n";

    cout << "Enter your name: ";
    getline(cin, studentName);

    while (studentName.empty())
    {
        cout << "Name cannot be empty. Please enter your name: ";
        getline(cin, studentName);
    }

    cout << "\nHello, " << studentName
         << "! Welcome to StudySphere.\n";

    dashboard(studentName);

    cout << "\nThank you for using StudySphere!\n";
    cout << "Keep learning. Keep growing.\n";

    return 0;
}
