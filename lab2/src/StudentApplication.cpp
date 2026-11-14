#include "StudentApplication.h"

#include <stdexcept>
#include <utility>

StudentApplication::StudentApplication(std::string studentId,
                                       std::string topic)
    : studentId_(std::move(studentId)), topic_(std::move(topic)) {
  if (studentId_.empty()) {
    throw std::invalid_argument("Student ID cannot be empty");
  }

  if (topic_.empty()) {
    throw std::invalid_argument("Application topic cannot be empty");
  }
}

void StudentApplication::markReviewed() {
  reviewed_ = true;
}

bool StudentApplication::reviewed() const {
  return reviewed_;
}

const std::string &StudentApplication::studentId() const {
  return studentId_;
}

const std::string &StudentApplication::topic() const {
  return topic_;
}
