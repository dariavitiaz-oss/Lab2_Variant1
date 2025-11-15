#pragma once
#include <iostream>
#include <string>

class STUDENT
{
private:
    std::string fullName;
    int* grades;
    int gradeCount;

public:
    STUDENT();
    STUDENT(const std::string& name, const int* values, int count);
    STUDENT(const STUDENT& other);
    ~STUDENT();

    STUDENT& operator=(const STUDENT& other);

    std::string getFullName() const;
    int getGradeCount() const;
    int getGrade(int index) const;
    double getAverage() const;

    void setFullName(const std::string& name);
    void setGrades(const int* values, int count);

    friend std::istream& operator>>(std::istream& in, STUDENT& student);
    friend std::ostream& operator<<(std::ostream& out, const STUDENT& student);
};
