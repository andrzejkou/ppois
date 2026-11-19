#pragma once

#include "Lesson.h"
#include "Exam.h"

#include <string>

class Subject;
class Classroom;

class Consultation : public Lesson {
public:
  Consultation(Subject subject, std::string teacherId, std::string day,
               int startHour, int durationHours, Classroom classroom,
               Exam consExam);
  void info() const override;

  const Exam &consExam() const;

private:
  Exam consExam_;
};
