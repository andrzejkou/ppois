#include "TransferOrder.h"

#include <stdexcept>
#include <utility>

TransferOrder::TransferOrder(int number, std::string studentId,
                             std::string fromGroup, std::string toGroup)
    : AcademicOrder(number, "Transfer order"),
      studentId_(std::move(studentId)),
      fromGroup_(std::move(fromGroup)),
      toGroup_(std::move(toGroup)) {
  if (studentId_.empty()) {
    throw std::invalid_argument("Student ID cannot be empty");
  }

  if (fromGroup_.empty() || toGroup_.empty()) {
    throw std::invalid_argument("Group codes cannot be empty");
  }

  if (fromGroup_ == toGroup_) {
    throw std::invalid_argument("Source and target groups must differ");
  }
}

const std::string &TransferOrder::studentId() const {
  return studentId_;
}

const std::string &TransferOrder::fromGroup() const {
  return fromGroup_;
}

const std::string &TransferOrder::toGroup() const {
  return toGroup_;
}
