#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "STUDENT.h"
#include "GROUP.h"
#include "TextProcessor.h"

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& prompt)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
            return value;

        std::cout << "Invalid input. Enter an integer." << std::endl;
        clearInput();
    }
}

int findGroup(const GROUP* groups, int count, int number)
{
    for (int i = 0; i < count; ++i)
        if (groups[i].getGroupNumber() == number)
            return i;
    return -1;
}

void sortGroups(GROUP* groups, int count)
{
    for (int i = 0; i < count - 1; ++i)
    {
        for (int j = 0; j < count - i - 1; ++j)
        {
            if (groups[j].getGroupNumber() > groups[j + 1].getGroupNumber())
            {
                GROUP temp(groups[j]);
                groups[j] = groups[j + 1];
                groups[j + 1] = temp;
            }
        }
    }
}

void listGroups(const GROUP* groups, int count)
{
    if (count == 0)
    {
        std::cout << "There are no groups." << std::endl;
        return;
    }

    for (int i = 0; i < count; ++i)
    {
        std::cout << "\nPosition: " << (i + 1) << std::endl;
        std::cout << groups[i] << std::endl;
    }
}

void addGroup(GROUP*& groups, int& count, int& capacity)
{
    int number = readInt("Enter group number: ");
    if (number <= 0)
        throw std::invalid_argument("Group number must be positive.");
    if (findGroup(groups, count, number) != -1)
        throw std::invalid_argument("A group with this number already exists.");

    int position = readInt("Insert position (1 to " + std::to_string(count + 1) + "): ");
    if (position < 1 || position > count + 1)
        throw std::out_of_range("Insertion position is invalid.");

    GROUP newGroup(number);

    int subjectCount = readInt("Enter number of subjects: ");
    if (subjectCount < 0 || subjectCount > 50)
        throw std::invalid_argument("Subject count must be from 0 to 50.");

    if (subjectCount > 0)
    {
        std::string* subjects = new std::string[subjectCount];
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (int i = 0; i < subjectCount; ++i)
        {
            std::cout << "Enter subject " << (i + 1) << ": ";
            std::getline(std::cin, subjects[i]);
        }
        newGroup.setSubjects(subjects, subjectCount);
        delete[] subjects;
    }

    if (count == capacity)
    {
        int newCapacity = (capacity == 0) ? 2 : capacity * 2;
        GROUP* expanded = new GROUP[newCapacity];
        for (int i = 0; i < count; ++i)
            expanded[i] = groups[i];
        delete[] groups;
        groups = expanded;
        capacity = newCapacity;
    }

    int index = position - 1;
    for (int i = count; i > index; --i)
        groups[i] = groups[i - 1];

    groups[index] = newGroup;
    ++count;
    sortGroups(groups, count);

    std::cout << "Group added successfully." << std::endl;
}

void addStudentToGroup(GROUP* groups, int count)
{
    int number = readInt("Enter group number: ");
    int index = findGroup(groups, count, number);
    if (index == -1)
        throw std::runtime_error("Group was not found.");

    STUDENT student;
    std::cin >> student;
    if (!std::cin)
    {
        clearInput();
        throw std::runtime_error("Student data is invalid.");
    }

    int position = readInt("Insert student position (1 to " +
                           std::to_string(groups[index].getStudentCount() + 1) + "): ");
    groups[index].addStudent(student, position - 1);
    std::cout << "Student added successfully." << std::endl;
}

void removeStudentFromGroup(GROUP* groups, int count)
{
    int number = readInt("Enter group number: ");
    int index = findGroup(groups, count, number);
    if (index == -1)
        throw std::runtime_error("Group was not found.");

    int position = readInt("Enter student position to remove: ");
    groups[index].removeStudent(position - 1);
    std::cout << "Student removed successfully." << std::endl;
}

void editStudentInGroup(GROUP* groups, int count)
{
    int number = readInt("Enter group number: ");
    int index = findGroup(groups, count, number);
    if (index == -1)
        throw std::runtime_error("Group was not found.");

    int position = readInt("Enter student position to edit: ");
    STUDENT replacement;
    std::cin >> replacement;
    if (!std::cin)
    {
        clearInput();
        throw std::runtime_error("Student data is invalid.");
    }

    groups[index].editStudent(position - 1, replacement);
    std::cout << "Student updated successfully." << std::endl;
}

void removeGroup(GROUP*& groups, int& count)
{
    if (count == 0)
        throw std::runtime_error("There are no groups to remove.");

    int position = readInt("Enter group position to remove: ");
    if (position < 1 || position > count)
        throw std::out_of_range("Group position is invalid.");

    for (int i = position - 1; i < count - 1; ++i)
        groups[i] = groups[i + 1];

    --count;
    std::cout << "Group removed successfully." << std::endl;
}

void editGroup(GROUP* groups, int count)
{
    if (count == 0)
        throw std::runtime_error("There are no groups to edit.");

    int position = readInt("Enter group position to edit: ");
    if (position < 1 || position > count)
        throw std::out_of_range("Group position is invalid.");

    int newNumber = readInt("Enter new group number: ");
    if (newNumber <= 0)
        throw std::invalid_argument("Group number must be positive.");

    int existing = findGroup(groups, count, newNumber);
    if (existing != -1 && existing != position - 1)
        throw std::invalid_argument("A group with this number already exists.");

    groups[position - 1].setGroupNumber(newNumber);

    int subjectCount = readInt("Enter new number of subjects: ");
    if (subjectCount < 0 || subjectCount > 50)
        throw std::invalid_argument("Subject count must be from 0 to 50.");

    std::string* subjects = nullptr;
    if (subjectCount > 0)
    {
        subjects = new std::string[subjectCount];
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (int i = 0; i < subjectCount; ++i)
        {
            std::cout << "Enter subject " << (i + 1) << ": ";
            std::getline(std::cin, subjects[i]);
        }
    }

    groups[position - 1].setSubjects(subjects, subjectCount);
    delete[] subjects;

    sortGroups(groups, count);
    std::cout << "Group updated successfully." << std::endl;
}

void showAllHighAchievers(const GROUP* groups, int count)
{
    bool found = false;

    for (int i = 0; i < count; ++i)
    {
        if (groups[i].hasHighAchievers())
        {
            groups[i].displayHighAchievers();
            found = true;
        }
    }

    if (!found)
        std::cout << "No students with an average grade above 4.0 were found." << std::endl;
}

void printMenu()
{
    std::cout << "\n========== LAB 2 - VARIANT 1 ==========\n"
              << "1. Add a group\n"
              << "2. Add a student to a group\n"
              << "3. Edit a student\n"
              << "4. Remove a student\n"
              << "5. Remove a group\n"
              << "6. Edit group information\n"
              << "7. Display all groups\n"
              << "8. Display all students with average above 4.0\n"
              << "9. Process a text file (swap adjacent words)\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

int main()
{
    GROUP* groups = nullptr;
    int groupCount = 0;
    int groupCapacity = 0;
    TextProcessor processor;

    bool running = true;
    while (running)
    {
        printMenu();
        int choice;

        if (!(std::cin >> choice))
        {
            std::cout << "Invalid menu input." << std::endl;
            clearInput();
            continue;
        }

        try
        {
            switch (choice)
            {
            case 1:
                addGroup(groups, groupCount, groupCapacity);
                break;
            case 2:
                addStudentToGroup(groups, groupCount);
                break;
            case 3:
                editStudentInGroup(groups, groupCount);
                break;
            case 4:
                removeStudentFromGroup(groups, groupCount);
                break;
            case 5:
                removeGroup(groups, groupCount);
                break;
            case 6:
                editGroup(groups, groupCount);
                break;
            case 7:
                listGroups(groups, groupCount);
                break;
            case 8:
                if (groupCount == 0)
                    std::cout << "There are no groups." << std::endl;
                else
                    showAllHighAchievers(groups, groupCount);
                break;
            case 9:
                processor.swapAdjacentWords("input.txt", "output.txt");
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Unknown menu option." << std::endl;
            }
        }
        catch (const std::exception& error)
        {
            std::cout << "Error: " << error.what() << std::endl;
        }
    }

    delete[] groups;
    groups = nullptr;

    std::cout << "Program finished." << std::endl;
    return 0;
}
