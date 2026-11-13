#include "Mark.h"

#include "exceptions.h"

#include <utility>

Mark::Mark(int value, Subject subject, Day day)
    : value_(value), subject_(std::move(subject)), day_(std::move(day)) {

  if (value < 0 || value > 10) {
    throw InvalidGradeException("Grade must be between 0 and 10");
  }
}

const Day &Mark::day() const { return day_; }

const Subject &Mark::subject() const { return subject_; }

int Mark::value() const { return value_; }
