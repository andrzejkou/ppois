#include "Benefit.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

Benefit::Benefit(std::string name, double amount, std::string description)
    : Premy(std::move(name), amount), description_(std::move(description)) {

  if (description_.empty()) {
    throw std::invalid_argument("Benefit description cannot be empty");
  }
}

void Benefit::assignTo(const std::string &studentId) {
  if (studentId.empty()) {
    throw std::invalid_argument("Student ID cannot be empty");
  }

  if (assignedTo(studentId)) {
    throw std::invalid_argument("Benefit is already assigned to student");
  }

  studentIds_.push_back(studentId);
}

bool Benefit::assignedTo(const std::string &studentId) const {
  return std::find(studentIds_.begin(), studentIds_.end(), studentId) !=
         studentIds_.end();
}
