
#include "Day.h"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

Day::Day(std::string name, std::string date)
    : name_(std::move(name)), date_(std::move(date)) {
  if (name_.empty()) {
    throw std::invalid_argument("Day name cannot be empty");
  }

  if (date_.empty()) {
    throw std::invalid_argument("Day date cannot be empty");
  }
}

const std::string &Day::name() const { return name_; }

const std::string &Day::getDate() const { return date_; }

void Day::addLesson(const std::shared_ptr<Lesson> &lesson) {
  if (!lesson) {
    throw std::invalid_argument("Lesson cannot be null");
  }

  lessons_.push_back(lesson);
}

std::size_t Day::lessonCount() const { return lessons_.size(); }

bool Day::removeLesson(std::size_t index) {
  if (index >= lessons_.size()) {
    return false;
  }
  lessons_.erase(lessons_.begin() + static_cast<std::ptrdiff_t>(index));
  return true;
}

bool Day::hasTeacher(const std::string &teacherId) const {
  return std::any_of(lessons_.begin(), lessons_.end(),
                     [&teacherId](const auto &lesson) {
                       return lesson->teacherId() == teacherId;
                     });
}

bool Day::hasSubject(const std::string &subjectCode) const {
  return std::any_of(lessons_.begin(), lessons_.end(),
                     [&subjectCode](const auto &lesson) {
                       return lesson->subject().code() == subjectCode;
                     });
}

bool Day::hasTimeConflict(int startHour, int durationHours) const {
  if (startHour < 0 || durationHours <= 0 ||
      startHour + durationHours > 24) {
    throw std::invalid_argument("Invalid lesson time interval");
  }
  const int endHour = startHour + durationHours;
  return std::any_of(lessons_.begin(), lessons_.end(),
                     [startHour, endHour](const auto &lesson) {
                       const int existingEnd =
                           lesson->startHour() + lesson->durationHours();
                       return startHour < existingEnd &&
                              lesson->startHour() < endHour;
                     });
}

std::vector<std::shared_ptr<Lesson>>
Day::lessonsByTeacher(const std::string &teacherId) const {
  std::vector<std::shared_ptr<Lesson>> result;
  std::copy_if(lessons_.begin(), lessons_.end(),
               std::back_inserter(result),
               [&teacherId](const auto &lesson) {
                 return lesson->teacherId() == teacherId;
               });
  return result;
}
