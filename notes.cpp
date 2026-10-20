
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

#include "notes.h"

using namespace std;

struct Note
{
    string subject;
    string topic;
    string content;
};

const string NOTES_FILE = "data/notes.txt";

// Load notes from file
vector<Note> loadNotes()
{
    vector<Note> notes;
    ifstream file(NOTES_FILE);

    Note note;

    while (getline(file, note.subject, '|') &&
           getline(file, note.topic, '|') &&
           getline(file, note.content))
    {
        notes.push_back(note);
    }

    return notes;
}

// Save notes to file
void saveNotes(const vector<Note> &notes)
{
    ofstream file(NOTES_FILE);

    for (const Note &note : notes)
    {
        file << note.subject << "|"
             << note.topic << "|"
             << note.content << "\n";
    }
}

// Display all notes
void viewNotes()
{
    vector<Note> notes = loadNotes();

    if (notes.empty())
    {
        cout << "\n================================\n";
        cout << "          NO NOTES FOUND\n";
        cout << "================================\n";
        cout << "Please add some notes first!\n";
        return;
    }

    cout << "\n================================\n";
    cout << "           YOUR NOTES\n";
    cout << "================================\n";

    for (int i = 0; i < static_cast<int>(notes.size()); i++)
    {
        cout << "\nNote #" << i + 1 << '\n';
        cout << "Subject : " << notes[i].subject << '\n';
        cout << "Topic   : " << notes[i].topic << '\n';
        cout << "Content : " << notes[i].content << '\n';
        cout << "--------------------------------\n";
    }

    cout << "Total notes: " << notes.size() << '\n';

    {
        vector<Note> notes = loadNotes();

        if (notes.empty())
        {
            cout << "\nNo notes found.\n";
            return;
        }

        cout << "\n========== ALL NOTES ==========\n";

        for (size_t i = 0; i < notes.size(); i++)
        {
            cout << "\nNote " << i + 1 << "\n";
            cout << "Subject: " << notes[i].subject << "\n";
            cout << "Topic: " << notes[i].topic << "\n";
            cout << "Content: " << notes[i].content << "\n";
            cout << "--------------------------------\n";
        }
    }
}

// Add a new note
void addNote()
{
    Note note;

    cout << "\nEnter subject: ";
    getline(cin, note.subject);

    cout << "Enter topic: ";
    getline(cin, note.topic);

    cout << "Enter your content: ";
    getline(cin, note.content);

    if (note.subject.empty() ||
        note.topic.empty() ||
        note.content.empty() ||
        note.subject.find('|') != string::npos ||
        note.topic.find('|') != string::npos ||
        note.content.find('|') != string::npos)
    {
        cout << "\nInvalid input! Fields cannot be empty "
             << "or contain '|'.\n";
        return;
    }

    vector<Note> notes = loadNotes();
    notes.push_back(note);

    saveNotes(notes);

    cout << "\nNote added successfully!\n";
}

// Search for a note using subject or topic
int searchNote(const vector<Note> &notes)
{
    string keyword;

    cout << "\nEnter subject or topic to search: ";
    getline(cin, keyword);

    for (size_t i = 0; i < notes.size(); i++)
    {
        if (notes[i].subject == keyword ||
            notes[i].topic == keyword)
        {
            cout << "\nNote found!\n";
            cout << "Subject: " << notes[i].subject << "\n";
            cout << "Topic: " << notes[i].topic << "\n";
            cout << "Content: " << notes[i].content << "\n";

            return static_cast<int>(i);
        }
    }

    cout << "\nNote not found.\n";
    return -1;
}

// Search menu
void searchNoteMenu()
{
    vector<Note> notes = loadNotes();

    if (notes.empty())
    {
        cout << "\nNo notes available.\n";
        return;
    }

    searchNote(notes);
}

// Edit an existing note
void editNote()
{
    vector<Note> notes = loadNotes();

    if (notes.empty())
    {
        cout << "\nNo notes available to edit.\n";
        return;
    }

    int index = searchNote(notes);

    if (index == -1)
    {
        return;
    }

    cout << "\nEnter new subject: ";
    string newSubject;
    getline(cin, newSubject);

    cout << "Enter new topic: ";
    string newTopic;
    getline(cin, newTopic);

    cout << "Enter new content: ";
    string newContent;
    getline(cin, newContent);

    if (newSubject.empty() ||
        newTopic.empty() ||
        newContent.empty() ||
        newSubject.find('|') != string::npos ||
        newTopic.find('|') != string::npos ||
        newContent.find('|') != string::npos)
    {
        cout << "\nInvalid input! Note was not updated.\n";
        return;
    }

    notes[index].subject = newSubject;
    notes[index].topic = newTopic;
    notes[index].content = newContent;

    saveNotes(notes);

    cout << "\nNote updated successfully!\n";
}

// Delete a note
void deleteNote()
{
    vector<Note> notes = loadNotes();

    if (notes.empty())
    {
        cout << "\nNo notes available to delete.\n";
        return;
    }

    int index = searchNote(notes);

    if (index == -1)
    {
        return;
    }

    notes.erase(notes.begin() + index);

    saveNotes(notes);

    cout << "\nNote deleted successfully!\n";
}

// Notes menu
void notesMenu()
{
    int choice;

    do
    {
        cout << "\n========================================\n";
        cout << "               NOTES MANAGER\n";
        cout << "========================================\n";

        cout << "1. Add Notes\n";
        cout << "2. Edit Notes\n";
        cout << "3. Delete Notes\n";
        cout << "4. View All Notes\n";
        cout << "5. Exit\n";

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        cin.ignore(
            numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            addNote();
            break;

        case 2:
            editNote();
            break;

        case 3:
            deleteNote();
            break;

        case 4:
            viewNotes();
            break;

        case 5:
            cout << "\nReturning to Student Dashboard...\n";
            break;

        default:
            cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 5);
}
