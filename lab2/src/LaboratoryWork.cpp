#include "LaboratoryWork.h"

#include <stdexcept>
#include <utility>

LaboratoryWork::LaboratoryWork(std::string title, int maxScore, int number)
    : AcademicWork(std::move(title), maxScore), number_(number) {
  if (number_ <= 0) {
    throw std::invalid_argument("Laboratory work number must be positive");
  }
}

int LaboratoryWork::number() const {
  return number_;
}

std::string LaboratoryWork::type() const {
  return "Laboratory work";
}
