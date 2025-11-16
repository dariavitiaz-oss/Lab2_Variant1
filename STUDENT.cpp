#include "STUDENT.h"
#include <limits>
#include <stdexcept>

STUDENT::STUDENT() : fullName(""), grades(nullptr), gradeCount(0)
{
    std::cout << "[STUDENT] Default constructor called." << std::endl;
}

STUDENT::STUDENT(const std::string& name, const int* values, int count)
    : fullName(name), grades(nullptr), gradeCount(0)
{
    if (count < 0)
        throw std::invalid_argument("Grade count cannot be negative.");

    if (count > 0 && values == nullptr)
        throw std::invalid_argument("Grade array is missing.");

    if (count > 0)
    {
        grades = new int[count];
        gradeCount = count;
        for (int i = 0; i < count; ++i)
        {
            if (values[i] < 2 || values[i] > 5)
            {
                delete[] grades;
                grades = nullptr;
                gradeCount = 0;
                throw std::invalid_argument("A grade must be from 2 to 5.");
            }
            grades[i] = values[i];
        }
    }
    std::cout << "[STUDENT] Parameterized constructor called." << std::endl;
}

STUDENT::STUDENT(const STUDENT& other)
    : fullName(other.fullName), grades(nullptr), gradeCount(other.gradeCount)
{
    if (gradeCount > 0)
    {
        grades = new int[gradeCount];
        for (int i = 0; i < gradeCount; ++i)
            grades[i] = other.grades[i];
    }
    std::cout << "[STUDENT] Copy constructor called." << std::endl;
}

STUDENT::~STUDENT()
{
    std::cout << "[STUDENT] Destructor called for: "
              << (fullName.empty() ? "<unnamed>" : fullName) << std::endl;
    delete[] grades;
}

STUDENT& STUDENT::operator=(const STUDENT& other)
{
    if (this != &other)
    {
        int* newGrades = nullptr;
        if (other.gradeCount > 0)
        {
            newGrades = new int[other.gradeCount];
            for (int i = 0; i < other.gradeCount; ++i)
                newGrades[i] = other.grades[i];
        }

        delete[] grades;
        grades = newGrades;
        gradeCount = other.gradeCount;
        fullName = other.fullName;
    }
    return *this;
}

std::string STUDENT::getFullName() const
{
    return fullName;
}

int STUDENT::getGradeCount() const
{
    return gradeCount;
}

int STUDENT::getGrade(int index) const
{
    if (index < 0 || index >= gradeCount)
        throw std::out_of_range("Grade index is out of range.");
    return grades[index];
}

double STUDENT::getAverage() const
{
    if (gradeCount == 0)
        return 0.0;

    int sum = 0;
    for (int i = 0; i < gradeCount; ++i)
        sum += grades[i];

    return static_cast<double>(sum) / gradeCount;
}

void STUDENT::setFullName(const std::string& name)
{
    if (name.empty())
        throw std::invalid_argument("Student name cannot be empty.");
    fullName = name;
}

void STUDENT::setGrades(const int* values, int count)
{
    if (count <= 0 || values == nullptr)
        throw std::invalid_argument("At least one grade is required.");

    int* newGrades = new int[count];
    for (int i = 0; i < count; ++i)
    {
        if (values[i] < 2 || values[i] > 5)
        {
            delete[] newGrades;
            throw std::invalid_argument("A grade must be from 2 to 5.");
        }
        newGrades[i] = values[i];
    }

    delete[] grades;
    grades = newGrades;
    gradeCount = count;
}

std::istream& operator>>(std::istream& in, STUDENT& student)
{
    std::string name;
    int count = 0;

    in >> std::ws;
    std::cout << "Enter full name (surname and initials): ";
    std::getline(in, name);
    if (!in)
        return in;

    std::cout << "Enter number of grades: ";
    in >> count;

    if (!in || count <= 0 || count > 50)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    int* values = new int[count];
    for (int i = 0; i < count; ++i)
    {
        std::cout << "Enter grade " << (i + 1) << " (2-5): ";
        in >> values[i];
        if (!in || values[i] < 2 || values[i] > 5)
        {
            delete[] values;
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    try
    {
        student.setFullName(name);
        student.setGrades(values, count);
    }
    catch (...)
    {
        delete[] values;
        throw;
    }

    delete[] values;
    return in;
}

std::ostream& operator<<(std::ostream& out, const STUDENT& student)
{
    out << "Name: " << student.fullName << "\nGrades: ";
    for (int i = 0; i < student.gradeCount; ++i)
    {
        out << student.grades[i];
        if (i + 1 < student.gradeCount)
            out << ' ';
    }
    out << "\nAverage: " << student.getAverage();
    return out;
}
