#pragma once

#include "Exam.h"

#include <string>

class Subject;
class Classroom;

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
