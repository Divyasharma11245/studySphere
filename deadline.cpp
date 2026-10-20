
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>

#include "deadline.h"

using namespace std;

struct Deadline
{
    string title;
    string date;
};

const string DEADLINE_FILE = "data/deadlines.txt";

// Convert a date string into year, month, and day
bool parseDate(const string &date, int &year, int &month, int &day)
{
    if (date.length() != 10 ||
        date[4] != '-' || date[7] != '-')
    {
        return false;
    }

    try
    {
        year = stoi(date.substr(0, 4));
        month = stoi(date.substr(5, 2));
        day = stoi(date.substr(8, 2));
    }
    catch (...)
    {
        return false;
    }

    if (year < 1 || month < 1 || month > 12)
        return false;

    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31};

    // Leap year validation
    bool leapYear = (year % 400 == 0) ||
                    (year % 4 == 0 && year % 100 != 0);

    if (leapYear)
        daysInMonth[1] = 29;

    if (day < 1 || day > daysInMonth[month - 1])
        return false;

    return true;
}

// Return today's date in YYYY-MM-DD format
string getTodayDate()
{
    time_t now = time(nullptr);
    tm *localTime = localtime(&now);

    if (localTime == nullptr)
        return "";

    ostringstream date;

    date << setw(4) << setfill('0')
         << localTime->tm_year + 1900 << "-"
         << setw(2) << setfill('0')
         << localTime->tm_mon + 1 << "-"
         << setw(2) << setfill('0')
         << localTime->tm_mday;

    return date.str();
}

// Load deadlines from file
vector<Deadline> loadDeadlines()
{
    vector<Deadline> deadlines;
    ifstream file(DEADLINE_FILE);

    Deadline d;

    while (getline(file, d.title, '|') &&
           getline(file, d.date))
    {
        deadlines.push_back(d);
    }

    return deadlines;
}

// Save deadlines to file
void saveDeadlines(const vector<Deadline> &deadlines)
{
    ofstream file(DEADLINE_FILE);

    if (!file)
    {
        cout << "Error: Could not save deadlines.\n";
        return;
    }

    for (const Deadline &d : deadlines)
    {
        file << d.title << "|" << d.date << "\n";
    }
}

// Display all deadlines
void viewDeadlines()
{
    vector<Deadline> deadlines = loadDeadlines();

    if (deadlines.empty())
    {
        cout << "\nNo deadlines found.\n";
        return;
    }

    cout << "\n========== ALL DEADLINES ==========\n";

    for (size_t i = 0; i < deadlines.size(); i++)
    {
        cout << i + 1 << ". "
             << deadlines[i].title
             << " | Date: " << deadlines[i].date << "\n";
    }
}

// Add a deadline
void addDeadline()
{
    Deadline d;

    cout << "\nEnter deadline title: ";
    getline(cin, d.title);

    if (d.title.empty() || d.title.find('|') != string::npos)
    {
        cout << "Invalid title. Title cannot be empty or contain '|'.\n";
        return;
    }

    cout << "Enter deadline date (YYYY-MM-DD): ";
    getline(cin, d.date);

    int year, month, day;

    if (!parseDate(d.date, year, month, day))
    {
        cout << "Invalid date! Use YYYY-MM-DD, for example 2026-10-15.\n";
        return;
    }

    vector<Deadline> deadlines = loadDeadlines();
    deadlines.push_back(d);

    saveDeadlines(deadlines);

    cout << "\nDeadline added successfully!\n";
}

// Search for a deadline
int searchDeadline(const vector<Deadline> &deadlines)
{
    string title;

    cout << "\nEnter deadline title to search: ";
    getline(cin, title);

    for (size_t i = 0; i < deadlines.size(); i++)
    {
        if (deadlines[i].title == title)
        {
            cout << "Deadline found: "
                 << deadlines[i].title
                 << " | Date: " << deadlines[i].date << "\n";

            return static_cast<int>(i);
        }
    }

    cout << "Deadline not found.\n";
    return -1;
}

// Edit a deadline
void editDeadline()
{
    vector<Deadline> deadlines = loadDeadlines();

    if (deadlines.empty())
    {
        cout << "\nNo deadlines available to edit.\n";
        return;
    }

    int index = searchDeadline(deadlines);

    if (index == -1)
        return;

    string newTitle, newDate;

    cout << "Enter new title: ";
    getline(cin, newTitle);

    if (newTitle.empty() || newTitle.find('|') != string::npos)
    {
        cout << "Invalid title. Deadline was not updated.\n";
        return;
    }

    cout << "Enter new date (YYYY-MM-DD): ";
    getline(cin, newDate);

    int year, month, day;

    if (!parseDate(newDate, year, month, day))
    {
        cout << "Invalid date! Deadline was not updated.\n";
        return;
    }

    deadlines[index].title = newTitle;
    deadlines[index].date = newDate;

    saveDeadlines(deadlines);

    cout << "\nDeadline updated successfully!\n";
}

// Delete a deadline
void deleteDeadline()
{
    vector<Deadline> deadlines = loadDeadlines();

    if (deadlines.empty())
    {
        cout << "\nNo deadlines available to delete.\n";
        return;
    }

    int index = searchDeadline(deadlines);

    if (index == -1)
        return;

    deadlines.erase(deadlines.begin() + index);

    saveDeadlines(deadlines);

    cout << "\nDeadline deleted successfully!\n";
}

// Search menu
void searchDeadlineMenu()
{
    vector<Deadline> deadlines = loadDeadlines();

    if (deadlines.empty())
    {
        cout << "\nNo deadlines available.\n";
        return;
    }

    searchDeadline(deadlines);
}

// Display upcoming deadlines within the next 7 days
void upcomingDeadlines()
{
    vector<Deadline> deadlines = loadDeadlines();

    if (deadlines.empty())
    {
        cout << "\nNo deadlines found.\n";
        return;
    }

    string today = getTodayDate();

    if (today.empty())
    {
        cout << "\nCould not determine today's date.\n";
        return;
    }

    time_t now = time(nullptr);
    tm *localTime = localtime(&now);

    if (localTime == nullptr)
    {
        cout << "\nCould not determine today's date.\n";
        return;
    }

    tm start = *localTime;
    start.tm_hour = 0;
    start.tm_min = 0;
    start.tm_sec = 0;
    start.tm_isdst = -1;

    time_t startTime = mktime(&start);

    tm end = start;
    end.tm_mday += 7;
    end.tm_isdst = -1;

    time_t endTime = mktime(&end);

    vector<Deadline> upcoming;

    for (const Deadline &d : deadlines)
    {
        int year, month, day;

        if (!parseDate(d.date, year, month, day))
            continue;

        tm deadlineTime = {};
        deadlineTime.tm_year = year - 1900;
        deadlineTime.tm_mon = month - 1;
        deadlineTime.tm_mday = day;
        deadlineTime.tm_isdst = -1;

        time_t dateValue = mktime(&deadlineTime);

        if (dateValue >= startTime && dateValue < endTime)
            upcoming.push_back(d);
    }

    sort(upcoming.begin(), upcoming.end(),
         [](const Deadline &a, const Deadline &b)
         {
             return a.date < b.date;
         });

    if (upcoming.empty())
    {
        cout << "\nNo upcoming deadlines in the next 7 days.\n";
        return;
    }

    cout << "\n========== UPCOMING DEADLINES ==========\n";

    for (size_t i = 0; i < upcoming.size(); i++)
    {
        cout << i + 1 << ". "
             << upcoming[i].title
             << " | Date: " << upcoming[i].date << "\n";
    }
}

// Display overdue deadlines
void overdueDeadlines()
{
    vector<Deadline> deadlines = loadDeadlines();

    if (deadlines.empty())
    {
        cout << "\nNo deadlines found.\n";
        return;
    }

    string today = getTodayDate();

    if (today.empty())
    {
        cout << "\nCould not determine today's date.\n";
        return;
    }

    vector<Deadline> overdue;

    for (const Deadline &d : deadlines)
    {
        int year, month, day;

        if (!parseDate(d.date, year, month, day))
            continue;

        // YYYY-MM-DD strings sort chronologically
        if (d.date < today)
            overdue.push_back(d);
    }

    sort(overdue.begin(), overdue.end(),
         [](const Deadline &a, const Deadline &b)
         {
             return a.date < b.date;
         });

    if (overdue.empty())
    {
        cout << "\nGreat! You have no overdue deadlines.\n";
        return;
    }

    cout << "\n========== OVERDUE DEADLINES ==========\n";

    for (size_t i = 0; i < overdue.size(); i++)
    {
        cout << i + 1 << ". "
             << overdue[i].title
             << " | Date: " << overdue[i].date
             << " | OVERDUE!\n";
    }
}

// Edit submenu
void editDeadlineMenu()
{
    int choice;

    do
    {
        cout << "\n========== EDIT DEADLINE ==========\n";
        cout << "1. Search Deadline\n";
        cout << "2. Edit Deadline\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            searchDeadlineMenu();
            break;
        case 2:
            editDeadline();
            break;
        case 3:
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

// Delete submenu
void deleteDeadlineMenu()
{
    int choice;

    do
    {
        cout << "\n========== DELETE DEADLINE ==========\n";
        cout << "1. Search Deadline\n";
        cout << "2. Delete Deadline\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            searchDeadlineMenu();
            break;
        case 2:
            deleteDeadline();
            break;
        case 3:
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

// Main deadline menu
void deadlineMenu()
{
    int choice;

    do
    {
        cout << "\n========================================\n";
        cout << "          DEADLINE MANAGEMENT\n";
        cout << "========================================\n";

        cout << "1. Add Deadline\n";
        cout << "2. Edit Deadline\n";
        cout << "3. Delete Deadline\n";
        cout << "4. View All Deadlines\n";
        cout << "5. Upcoming Deadlines (Next 7 Days)\n";
        cout << "6. Overdue Alerts\n";
        cout << "7. Exit\n";

        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            addDeadline();
            break;
        case 2:
            editDeadlineMenu();
            break;
        case 3:
            deleteDeadlineMenu();
            break;
        case 4:
            viewDeadlines();
            break;
        case 5:
            upcomingDeadlines();
            break;
        case 6:
            overdueDeadlines();
            break;
        case 7:
            cout << "Returning to Student Dashboard...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 7);
}
