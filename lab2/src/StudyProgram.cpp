#include "StudyProgram.h"
#include "Specialty.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

StudyProgram::StudyProgram(std::string name, int admissionYear)
    : name_(std::move(name)), admissionYear_(admissionYear) {
  if (name_.empty()) {
    throw std::invalid_argument("Study program name cannot be empty");
  }

  if (admissionYear_ <= 0) {
    throw std::invalid_argument("Admission year must be positive");
  }
}

void StudyProgram::addSpecialty(const Specialty &specialty) {
  if (offers(specialty.code())) {
    throw std::invalid_argument("Specialty already belongs to study program");
  }

  specialtyCodes_.push_back(specialty.code());
}

bool StudyProgram::offers(const std::string &specialtyCode) const {
  return std::find(specialtyCodes_.begin(), specialtyCodes_.end(),
                   specialtyCode) != specialtyCodes_.end();
}

int StudyProgram::admissionYear() const {
  return admissionYear_;
}

const std::string &StudyProgram::name() const {
  return name_;
}
