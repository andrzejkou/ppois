#pragma once

#include <string>
#include <vector>


class Subject {
public:
    Subject(std::string code, std::string name, int credits);
    const std::string& code() const;
    const std::string& name() const;
    int credits() const;

private:
    std::string code_;
    std::string name_;
    int credits_;
};

class Classroom {
public:
    Classroom(std::string roomNumber, int capacity);
    const std::string& roomNumber() const;
    int capacity() const;
    bool canFit(int peopleCount) const;

private:
    std::string roomNumber_;
    int capacity_;
};

class Lesson {
public:
    Lesson(Subject subject, std::string teacherId, std::string day, int startHour, int durationHours);
    virtual ~Lesson() = default;
    virtual std::string format() const;
    const Subject& subject() const;
    const std::string& teacherId() const;
    const std::string& day() const;
    int startHour() const;
    int durationHours() const;

private:
    Subject subject_;
    std::string teacherId_;
    std::string day_;
    int startHour_;
    int durationHours_;
};

class Lecture : public Lesson {
public:
    using Lesson::Lesson;
    std::string format() const override;
};
class Seminar : public Lesson {
public:
    using Lesson::Lesson;
    std::string format() const override;
};
class Exam : public Lesson {
public:
    Exam(Subject subject, std::string teacherId, std::string day, int startHour,
         int durationHours, std::string examType);
    const std::string& examType() const;

private:
    std::string examType_;
};

class Semester {
public:
    Semester(int number, int year);
    void addSubject(const Subject& subject);
    bool hasSubject(const std::string& subjectCode) const;
    int number() const;
    int year() const;

private:
    int number_;
    int year_;
    std::vector<Subject> subjects_;
};

class Schedule {
public:
    explicit Schedule(std::string groupCode);
    void addLesson(const Lesson& lesson);
    std::size_t lessonCount() const;
    const std::string& groupCode() const;

private:
    std::string groupCode_;
    std::vector<Lesson> lessons_;
};

