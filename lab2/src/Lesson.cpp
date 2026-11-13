#include "Lesson.h"

#include <iostream>
#include <stdexcept>
#include <utility>

Lesson::Lesson(Subject subject, std::string teacherId, std::string day,
               int startHour, int durationHours, Classroom classroom)
    : subject_(std::move(subject)), teacherId_(std::move(teacherId)),
      day_(std::move(day)), startHour_(startHour),
      durationHours_(durationHours), classroom_(std::move(classroom)) {

  if (teacherId_.empty()) {
    throw std::invalid_argument("Teacher ID cannot be empty");
  }

  if (day_.empty()) {
    throw std::invalid_argument("Day cannot be empty");
  }

  if (startHour_ < 0 || startHour_ > 23) {
    throw std::invalid_argument("Invalid lesson start hour");
  }

  if (durationHours_ <= 0 || startHour_ + durationHours_ > 24) {
    throw std::invalid_argument("Invalid lesson duration");
  }
}
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
