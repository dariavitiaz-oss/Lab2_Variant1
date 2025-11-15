#pragma once
#include "STUDENT.h"
#include <string>

class GROUP
{
private:
    int groupNumber;
    std::string* subjects;
    int subjectCount;
    STUDENT* students;
    int studentCount;

    void copyFrom(const GROUP& other);

public:
    GROUP();
    explicit GROUP(int number);
    GROUP(const GROUP& other);
    ~GROUP();

    GROUP& operator=(const GROUP& other);

    int getGroupNumber() const;
    int getStudentCount() const;
    int getSubjectCount() const;
    double getGroupAverage() const;

    void setGroupNumber(int number);
    void setSubjects(const std::string* names, int count);

    void addSubject(const std::string& subject);
    void addStudent(const STUDENT& student, int position);
    void removeStudent(int position);
    void editStudent(int position, const STUDENT& student);
    void displayStudents() const;
    bool hasHighAchievers() const;
    void displayHighAchievers() const;

    friend std::ostream& operator<<(std::ostream& out, const GROUP& group);
};
