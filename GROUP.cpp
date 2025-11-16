#include "GROUP.h"
#include <iostream>
#include <stdexcept>

GROUP::GROUP()
    : groupNumber(0), subjects(nullptr), subjectCount(0),
      students(nullptr), studentCount(0)
{
    std::cout << "[GROUP] Default constructor called." << std::endl;
}

GROUP::GROUP(int number)
    : groupNumber(number), subjects(nullptr), subjectCount(0),
      students(nullptr), studentCount(0)
{
    if (number <= 0)
        throw std::invalid_argument("Group number must be positive.");
    std::cout << "[GROUP] Parameterized constructor called." << std::endl;
}

void GROUP::copyFrom(const GROUP& other)
{
    groupNumber = other.groupNumber;
    subjectCount = other.subjectCount;
    studentCount = other.studentCount;

    subjects = nullptr;
    students = nullptr;

    if (subjectCount > 0)
    {
        subjects = new std::string[subjectCount];
        for (int i = 0; i < subjectCount; ++i)
            subjects[i] = other.subjects[i];
    }

    if (studentCount > 0)
    {
        students = new STUDENT[studentCount];
        for (int i = 0; i < studentCount; ++i)
            students[i] = other.students[i];
    }
}

GROUP::GROUP(const GROUP& other)
    : groupNumber(0), subjects(nullptr), subjectCount(0),
      students(nullptr), studentCount(0)
{
    copyFrom(other);
    std::cout << "[GROUP] Copy constructor called." << std::endl;
}

GROUP::~GROUP()
{
    std::cout << "[GROUP] Destructor called for group "
              << groupNumber << "." << std::endl;
    delete[] subjects;
    delete[] students;
}

GROUP& GROUP::operator=(const GROUP& other)
{
    if (this != &other)
    {
        std::string* newSubjects = nullptr;
        STUDENT* newStudents = nullptr;

        if (other.subjectCount > 0)
        {
            newSubjects = new std::string[other.subjectCount];
            for (int i = 0; i < other.subjectCount; ++i)
                newSubjects[i] = other.subjects[i];
        }

        if (other.studentCount > 0)
        {
            newStudents = new STUDENT[other.studentCount];
            for (int i = 0; i < other.studentCount; ++i)
                newStudents[i] = other.students[i];
        }

        delete[] subjects;
        delete[] students;

        subjects = newSubjects;
        students = newStudents;
        subjectCount = other.subjectCount;
        studentCount = other.studentCount;
        groupNumber = other.groupNumber;
    }
    return *this;
}

int GROUP::getGroupNumber() const { return groupNumber; }
int GROUP::getStudentCount() const { return studentCount; }
int GROUP::getSubjectCount() const { return subjectCount; }

double GROUP::getGroupAverage() const
{
    if (studentCount == 0)
        return 0.0;

    double sum = 0.0;
    for (int i = 0; i < studentCount; ++i)
        sum += students[i].getAverage();

    return sum / studentCount;
}

void GROUP::setGroupNumber(int number)
{
    if (number <= 0)
        throw std::invalid_argument("Group number must be positive.");
    groupNumber = number;
}

void GROUP::setSubjects(const std::string* names, int count)
{
    if (count < 0 || (count > 0 && names == nullptr))
        throw std::invalid_argument("Invalid subject list.");

    std::string* newSubjects = nullptr;
    if (count > 0)
    {
        newSubjects = new std::string[count];
        for (int i = 0; i < count; ++i)
        {
            if (names[i].empty())
            {
                delete[] newSubjects;
                throw std::invalid_argument("Subject name cannot be empty.");
            }
            newSubjects[i] = names[i];
        }
    }

    delete[] subjects;
    subjects = newSubjects;
    subjectCount = count;
}

void GROUP::addSubject(const std::string& subject)
{
    if (subject.empty())
        throw std::invalid_argument("Subject name cannot be empty.");

    std::string* newSubjects = new std::string[subjectCount + 1];
    for (int i = 0; i < subjectCount; ++i)
        newSubjects[i] = subjects[i];

    newSubjects[subjectCount] = subject;
    delete[] subjects;
    subjects = newSubjects;
    ++subjectCount;
}

void GROUP::addStudent(const STUDENT& student, int position)
{
    if (position < 0 || position > studentCount)
        throw std::out_of_range("Student insertion position is invalid.");

    if (subjectCount > 0 && student.getGradeCount() != subjectCount)
        throw std::invalid_argument("The number of grades must match the number of group subjects.");

    STUDENT* newStudents = new STUDENT[studentCount + 1];

    for (int i = 0; i < position; ++i)
        newStudents[i] = students[i];

    newStudents[position] = student;

    for (int i = position; i < studentCount; ++i)
        newStudents[i + 1] = students[i];

    delete[] students;
    students = newStudents;
    ++studentCount;
}

void GROUP::removeStudent(int position)
{
    if (position < 0 || position >= studentCount)
        throw std::out_of_range("Student position is invalid.");

    STUDENT* newStudents = nullptr;
    if (studentCount > 1)
    {
        newStudents = new STUDENT[studentCount - 1];
        int target = 0;
        for (int i = 0; i < studentCount; ++i)
        {
            if (i != position)
                newStudents[target++] = students[i];
        }
    }

    delete[] students;
    students = newStudents;
    --studentCount;
}

void GROUP::editStudent(int position, const STUDENT& student)
{
    if (position < 0 || position >= studentCount)
        throw std::out_of_range("Student position is invalid.");

    if (subjectCount > 0 && student.getGradeCount() != subjectCount)
        throw std::invalid_argument("The number of grades must match the number of group subjects.");

    students[position] = student;
}

void GROUP::displayStudents() const
{
    if (studentCount == 0)
    {
        std::cout << "This group has no students." << std::endl;
        return;
    }

    for (int i = 0; i < studentCount; ++i)
    {
        std::cout << "\nStudent position: " << (i + 1) << std::endl;
        std::cout << students[i] << std::endl;
    }
}

bool GROUP::hasHighAchievers() const
{
    for (int i = 0; i < studentCount; ++i)
        if (students[i].getAverage() > 4.0)
            return true;
    return false;
}

void GROUP::displayHighAchievers() const
{
    bool found = false;
    for (int i = 0; i < studentCount; ++i)
    {
        if (students[i].getAverage() > 4.0)
        {
            std::cout << students[i].getFullName()
                      << " | Group: " << groupNumber
                      << " | Average: " << students[i].getAverage()
                      << std::endl;
            found = true;
        }
    }
    (void)found;
}

std::ostream& operator<<(std::ostream& out, const GROUP& group)
{
    out << "Group number: " << group.groupNumber << "\nSubjects: ";
    if (group.subjectCount == 0)
        out << "not specified";
    else
    {
        for (int i = 0; i < group.subjectCount; ++i)
        {
            out << group.subjects[i];
            if (i + 1 < group.subjectCount)
                out << ", ";
        }
    }
    out << "\nStudent count: " << group.studentCount
        << "\nGroup average: " << group.getGroupAverage();
    return out;
}
