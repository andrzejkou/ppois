#include "schedule.h"

Exam::Exam(Subject subject, std::string teacherId, std::string day,
           int startHour, int durationHours, std::string examType)
    : Lesson(Subject subject, std::string teacherId, std::string day,
             int startHour, int durationHours),
      std::string examType() {}
