
#include "Day.h"

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
