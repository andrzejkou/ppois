
#include "Semester.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

Semester::Semester(int number, int year) : number_(number), year_(year) {
  if (number_ < 1 || number_ > 2) {
    throw std::invalid_argument("Semester number must be 1 or 2");
  }

  if (year_ <= 0) {
    throw std::invalid_argument("Academic year must be positive");
  }
}

void Semester::addSubject(const Subject &subject) {
  if (hasSubject(subject.code())) {
    throw std::invalid_argument("Subject already exists in this semester");
  }

  subjects_.push_back(subject);
}

bool Semester::hasSubject(const std::string &subjectCode) const {
  return std::any_of(subjects_.begin(), subjects_.end(),
                     [&subjectCode](const Subject &subject) {
                       return subject.code() == subjectCode;
                     });
}

int Semester::number() const { return number_; }

int Semester::year() const { return year_; }
