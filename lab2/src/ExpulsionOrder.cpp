#include "ExpulsionOrder.h"

#include <stdexcept>
#include <utility>

ExpulsionOrder::ExpulsionOrder(int number, std::string studentId,
                               std::string reason)
    : AcademicOrder(number, "Expulsion order"),
      studentId_(std::move(studentId)),
      reason_(std::move(reason)) {
  if (studentId_.empty()) {
    throw std::invalid_argument("Student ID cannot be empty");
  }

  if (reason_.empty()) {
    throw std::invalid_argument("Expulsion reason cannot be empty");
  }
}

const std::string &ExpulsionOrder::studentId() const {
  return studentId_;
}

const std::string &ExpulsionOrder::reason() const {
  return reason_;
}
