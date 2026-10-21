#pragma once

#include <string>
#include <vector>


class People {
public:
    People(std::string fullName, int age);
    virtual ~People() = default;

    const std::string& fullName() const;
    int age() const;
    virtual std::string role() const;
    void changeName(const std::string& newName);

protected:
    std::string fullName_;
    int age_;
};

class Student : public People {
public:
    Student(std::string fullName, int age, std::string studentId, int year);
    const std::string& studentId() const;
    int year() const;
    void promoteToNextYear();
    void setAcademicDebtCount(int count);
    int academicDebtCount() const;
    bool isEligibleForScholarship() const;
    std::string role() const override;

private:
    std::string studentId_;
    int year_;
    int academicDebtCount_{0};
};

class Worker : public People {
public:
    Worker(std::string fullName, int age, std::string employeeId, std::string position);
    const std::string& employeeId() const;
    const std::string& position() const;
    void changePosition(const std::string& position);
    std::string role() const override;

protected:
    std::string employeeId_;
    std::string position_;
};

class Dean : public Worker {
public:
    Dean(std::string fullName, int age, std::string employeeId);
    bool approveOrder(const std::string& orderTitle) const;
    std::string role() const override;
};

class Secretary : public Worker {
public:
    Secretary(std::string fullName, int age, std::string employeeId);
    std::string registerDocument(const std::string& title) const;
};

class Teacher : public Worker {
public:
    Teacher(std::string fullName, int age, std::string employeeId, std::string department);
    const std::string& department() const;
    void assignCourse(const std::string& courseName);
    const std::vector<std::string>& courses() const;
    std::string role() const override;

private:
    std::string department_;
    std::vector<std::string> courses_;
};

class Curator : public Worker {
public:
    Curator(std::string fullName, int age, std::string employeeId, std::string groupCode);
    const std::string& groupCode() const;
    void assignGroup(const std::string& groupCode);

private:
    std::string groupCode_;
};

