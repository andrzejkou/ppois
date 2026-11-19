#include "AcademicWork.h"

#include <stdexcept>
#include <utility>

AcademicWork::AcademicWork(std::string title, int maxScore)
    : title_(std::move(title)), maxScore_(maxScore) {
  if (title_.empty()) {
    throw std::invalid_argument("Academic work title cannot be empty");
  }

  if (maxScore_ <= 0) {
    throw std::invalid_argument("Maximum score must be positive");
  }
}

std::string AcademicWork::type() const {
  return "Academic work";
}

const std::string &AcademicWork::title() const {
  return title_;
}

int AcademicWork::maxScore() const {
  return maxScore_;
}
