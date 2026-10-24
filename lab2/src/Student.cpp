#include "people.h"
#include <iostream>
#include <string>

Student::Student(Student &&other) noexcept
    : People(std::move(other)), studentId_(std::move(other.studentId_)),
      year_(other.year_), academicDebtCount_(other.academicDebtCount_) {
  other.year_ = 0;
  other.studentId_.clear();
  other.academicDebtCount_ = 0;
}

const std::string &Student::studentId() const { return studentId_; }

int Student::year() const { return year_; }

void Student::printInfo() const {
  std::cout << "Role: " << role() << '\n'
            << "Full name: " << fullName() << '\n'
            << "Age: " << age() << '\n'
            << "Student ID: " << studentId_ << '\n'
            << "Year: " << year_ << '\n'
            << "Academic debts: " << academicDebtCount_ << '\n'
            << "Eligible for scholarship: "
            << (isEligibleForScholarship() ? "Yes" : "No") << '\n';
}

std::string Student::role() const { return "student"; }

void Student::promoteToNextYear() { ++year_; }

int Student::academicDebtCount() const { return academicDebtCount_; }

void Student::setAcademicDebtCount(int count) {
  if (count >= 0) {
    academicDebtCount_ = count;
  }
}

bool Student::isEligibleForScholarship() const {
  if (academicDebtCount_ > 0)
    return false;
  return true;
}
