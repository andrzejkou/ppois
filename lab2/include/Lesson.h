#pragma once

#include "Classroom.h"
#include "Subject.h"

#include <string>

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
