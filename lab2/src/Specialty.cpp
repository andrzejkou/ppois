#include "Specialty.h"
#include "Curriculum.h"

#include <stdexcept>
#include <utility>

Specialty::Specialty(std::string code, std::string name, int durationYears)
    : code_(std::move(code)),
      name_(std::move(name)),
      durationYears_(durationYears) {
  if (code_.empty()) {
    throw std::invalid_argument("Specialty code cannot be empty");
  }

  if (name_.empty()) {
    throw std::invalid_argument("Specialty name cannot be empty");
  }

  if (durationYears_ <= 0) {
    throw std::invalid_argument("Specialty duration must be positive");
  }
}

void Specialty::attachCurriculum(const Curriculum &curriculum) {
  curriculumCode_ = curriculum.code();
  hasCurriculum_ = true;
}

bool Specialty::hasCurriculum() const {
  return hasCurriculum_;
}

const std::string &Specialty::code() const {
  return code_;
}

const std::string &Specialty::name() const {
  return name_;
}

int Specialty::durationYears() const {
  return durationYears_;
}
