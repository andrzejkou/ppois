#pragma once

#include <memory>
#include <string>
#include <vector>

class Subject {
public:
  Subject(std::string code, std::string name, int credits);
  const std::string &code() const;
  const std::string &name() const;
  int credits() const;

private:
  std::string code_;
  std::string name_;
  int credits_;
};

class Classroom {
public:
  Classroom(std::string roomNumber, int capacity);
  const std::string &roomNumber() const;
  int capacity() const;
  bool canFit(int peopleCount) const;

private:
  std::string roomNumber_;
  int capacity_;
};

class Lesson {
public:
  Lesson(Subject subject, std::string teacherId, std::string day, int startHour,
         int durationHours, Classroom classroom);
  virtual ~Lesson() = default;
  virtual void info() const = 0;
  const Subject &subject() const;
  const std::string &teacherId() const;
  const std::string &day() const;
  int startHour() const;
  int durationHours() const;

private:
  Subject subject_;
  std::string teacherId_;
  std::string day_;
  int startHour_;
  int durationHours_;
  Classroom classroom_;

protected:
  void baseInfo() const;
};

class Lecture : public Lesson {
public:
  using Lesson::Lesson;
  void info() const override;
};
class Seminar : public Lesson {
public:
  using Lesson::Lesson;
  void info() const override;
};
class Exam : public Lesson {
public:
  Exam(Subject subject, std::string teacherId, std::string day, int startHour,
       int durationHours, Classroom classroom, std::string examType);
  const std::string &examType() const;
  void info() const override;
  void examTypeSet(std::string &examTypeEx);

private:
  std::string examType_;
};

class CourseProjCheck : public Lesson {
public:
  using Lesson::Lesson;
  void info() const override;
};

class Consultation : public Lesson {
public:
  Consultation(Subject subject, std::string teacherId, std::string day,
               int startHour, int durationHours, Classroom classroom,
               Exam consExam);
  void info() const override;

  const Exam consExam() const;

private:
  Exam consExam_;
};

class Perezdacha : public Exam {
public:
  Perezdacha(Subject subject, std::string teacherId, std::string day,
             int startHour, int durationHours, Classroom classroom,
             std::string examType, int number);
  void info() const override;
  int number();

private:
  int number_;
};
class Semester {
public:
  Semester(int number, int year);
  void addSubject(const Subject &subject);
  bool hasSubject(const std::string &subjectCode) const;
  int number() const;
  int year() const;

private:
  int number_;
  int year_;
  std::vector<Subject> subjects_;
};

class Day {
public:
  const std::string &name() const;

private:
  std::string name_;
  std::string date;
  std::vector<std::unique_ptr<Lesson>> lessons_;
};

class Week {
public:
  Week(int number);
  void addDay(const Day &day);
  int number() const;

private:
  int number_;
  std::vector<Day> days_;
};

class Schedule {
public:
  explicit Schedule(std::string groupCode);
  void addWeek(const Week &weekEx);
  void changeSchedule();
  std::size_t lessonCount() const;
  const std::string &groupCode() const;

private:
  std::string groupCode_;
  std::vector<Week> weeks_;
};
