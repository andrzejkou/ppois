#include "schedule.h"
#include <iostream>
Lesson::Lesson(Subject subject, std::string teacherId, std::string day,
               int startHour, int durationHours, Classroom classroom)
    : subject_(subject), teacherId_(teacherId), day_(day),
      startHour_(startHour), durationHours_(durationHours),
      classroom_(classroom) {}

const Subject &Lesson::subject() const { return subject_; }

const std::string &Lesson::teacherId() const { return teacherId_; }

const std::string &Lesson::day() const { return day_; }

int Lesson::startHour() const { return startHour_; }

int Lesson::durationHours() const { return durationHours_; }

void Lesson::baseInfo() const {
  std::cout << subject_.name() << "\nПреподаватель: " << teacherId_
            << "\nДень: " << day_ << "\nВремя: " << startHour_ << ":00 - "
            << startHour_ + durationHours_ << ":00\n";
}
