#pragma once

#include "Lesson.h"

#include <string>

class Subject;
class Classroom;

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
