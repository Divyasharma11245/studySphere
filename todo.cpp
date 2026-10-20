
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

#include "todo.h"

using namespace std;

struct Task
{
    string title;
    string status;
};

const string TASK_FILE = "data/tasks.txt";

// Load tasks from file
vector<Task> loadTasks()
{
    vector<Task> tasks;
    ifstream file(TASK_FILE);

    Task task;

    while (getline(file, task.title, '|') &&
           getline(file, task.status))
    {
        tasks.push_back(task);
    }

    return tasks;
}

// Save tasks to file
bool saveTasks(const vector<Task> &tasks)
{
    ofstream file(TASK_FILE);

    if (!file)
    {
        cout << "Error: Could not save tasks.\n";
        return false;
    }

    for (const Task &task : tasks)
    {
        file << task.title << "|"
             << task.status << "\n";
    }

    if (!file)
    {
        cout << "Error: Failed while saving tasks.\n";
        return false;
    }

    return true;
}

// Display all tasks
void viewTasks()
{
    vector<Task> tasks = loadTasks();

    if (tasks.empty())
    {
        cout << "\nNo tasks found. Please add a task first.\n";
        return;
    }

    cout << "\n========== ALL TASKS ==========\n";

    for (size_t i = 0; i < tasks.size(); i++)
    {
        cout << i + 1 << ". "
             << tasks[i].title
             << " [" << tasks[i].status << "]\n";
    }

    cout << "\nTotal tasks: " << tasks.size() << '\n';
}

// Add a task
void addTask()
{
    Task task;

    cout << "\nEnter task name: ";
    getline(cin, task.title);

    if (task.title.empty() ||
        task.title.find('|') != string::npos)
    {
        cout << "Invalid task name. It cannot be empty or contain '|'.\n";
        return;
    }

    task.status = "Pending";

    vector<Task> tasks = loadTasks();
    tasks.push_back(task);

    if (saveTasks(tasks))
    {
        cout << "\nTask added successfully!\n";
    }
}

// Search for a task by title
int searchTask(const vector<Task> &tasks)
{
    string title;

    cout << "\nEnter task name to search: ";
    getline(cin, title);

    for (size_t i = 0; i < tasks.size(); i++)
    {
        if (tasks[i].title == title)
        {
            cout << "\nTask found!\n";
            cout << "Name: " << tasks[i].title << '\n';
            cout << "Status: " << tasks[i].status << '\n';

            return static_cast<int>(i);
        }
    }

    cout << "\nTask not found. Please check the task name.\n";
    return -1;
}

// Search menu
void searchTaskMenu()
{
    vector<Task> tasks = loadTasks();

    if (tasks.empty())
    {
        cout << "\nNo tasks found. Please add a task first.\n";
        return;
    }

    searchTask(tasks);
}

// Display tasks by status
void viewTasksByStatus(const string &status)
{
    vector<Task> tasks = loadTasks();

    if (tasks.empty())
    {
        cout << "\nNo tasks found. Please add a task first.\n";
        return;
    }

    cout << "\n========== "
         << (status == "Pending" ? "PENDING TASKS" : "COMPLETED TASKS")
         << " ==========\n";

    int count = 0;

    for (const Task &task : tasks)
    {
        if (task.status == status)
        {
            cout << ++count << ". "
                 << task.title
                 << " [" << task.status << "]\n";
        }
    }

    if (count == 0)
    {
        cout << "No " << status << " tasks found.\n";
    }
    else
    {
        cout << "\nTotal: " << count << '\n';
    }
}

// Delete a task
void deleteTask()
{
    vector<Task> tasks = loadTasks();

    if (tasks.empty())
    {
        cout << "\nNo tasks found. Nothing to delete.\n";
        return;
    }

    int index = searchTask(tasks);

    if (index == -1)
        return;

    tasks.erase(tasks.begin() + index);

    if (saveTasks(tasks))
    {
        cout << "\nTask deleted successfully!\n";
    }
}

// Change a task's title or status
void changeTask()
{
    vector<Task> tasks = loadTasks();

    if (tasks.empty())
    {
        cout << "\nNo tasks found. Nothing to change.\n";
        return;
    }

    int index = searchTask(tasks);

    if (index == -1)
        return;

    int choice;

    cout << "\n========== CHANGE TASK ==========\n";
    cout << "1. Change Task Name\n";
    cout << "2. Change Task Status\n";
    cout << "3. Cancel\n";
    cout << "Enter your choice: ";

    if (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input!\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice)
    {
    case 1:
    {
        string newTitle;

        cout << "Enter new task name: ";
        getline(cin, newTitle);

        if (newTitle.empty() ||
            newTitle.find('|') != string::npos)
        {
            cout << "Invalid task name.\n";
            return;
        }

        tasks[index].title = newTitle;

        if (saveTasks(tasks))
        {
            cout << "\nTask name updated successfully!\n";
        }

        break;
    }

    case 2:
    {
        int statusChoice;

        cout << "\n1. Pending\n";
        cout << "2. Completed\n";
        cout << "Enter new status: ";

        if (!(cin >> statusChoice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (statusChoice == 1)
        {
            tasks[index].status = "Pending";
        }
        else if (statusChoice == 2)
        {
            tasks[index].status = "Completed";
        }
        else
        {
            cout << "Invalid status choice.\n";
            return;
        }

        if (saveTasks(tasks))
        {
            cout << "\nTask status updated successfully!\n";
        }

        break;
    }

    case 3:
        cout << "Task change cancelled.\n";
        break;

    default:
        cout << "Invalid choice!\n";
    }
}

// Main todo menu
void todoMenu()
{
    int choice;

    do
    {
        cout << "\n========================================\n";
        cout << "               TODO MANAGER\n";
        cout << "========================================\n";

        cout << "1. Add Task\n";
        cout << "2. Delete Task\n";
        cout << "3. View All Tasks\n";
        cout << "4. Change Task\n";
        cout << "5. Search Task\n";
        cout << "6. View Pending Tasks\n";
        cout << "7. View Completed Tasks\n";
        cout << "8. Exit\n";

        cout << "\nEnter your choice: ";

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
            addTask();
            break;

        case 2:
            deleteTask();
            break;

        case 3:
            viewTasks();
            break;

        case 4:
            changeTask();
            break;

        case 5:
            searchTaskMenu();
            break;

        case 6:
            viewTasksByStatus("Pending");
            break;

        case 7:
            viewTasksByStatus("Completed");
            break;

        case 8:
            cout << "\nReturning to Student Dashboard...\n";
            break;

        default:
            cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 8);
}
