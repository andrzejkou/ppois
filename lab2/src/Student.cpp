#include "Student.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
Student::Student(std::string fullName, int age, std::string studentId, int year)
    : People(std::move(fullName), age), studentId_(std::move(studentId)),
      subGroupNum_(0), year_(year), averageMark(0.0), academicDebtCount_(0) {}

Student::Student(Student &&other) noexcept
    : People(std::move(other)), studentId_(std::move(other.studentId_)),
      subGroupNum_(other.subGroupNum_), year_(other.year_),
      averageMark(other.averageMark),
      scholarship_(std::move(other.scholarship_)),
      benefit_(std::move(other.benefit_)), marks_(std::move(other.marks_)),
      academicDebtCount_(other.academicDebtCount_) {

  other.studentId_.clear();
  other.subGroupNum_ = 0;
  other.year_ = 0;
  other.averageMark = 0.0;
  other.academicDebtCount_ = 0;
}

double Student::averageMarkCheck() { return averageMark; }

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
void Student::removeScholarship() { scholarship_.reset(); }
void Student::awardScholarship() {
  int amount = 0;
  double average = averageMarkCheck();
  if (average >= 9) {
    amount = 2000;
  } else if (average >= 8) {
    amount = 1500;
  } else if (average >= 6) {
    amount = 1200;
  } else {
    amount = 1000;
  }
  if (scholarship_) {

    scholarship_->changeAmount(amount);
  } else
    scholarship_.emplace("Академическая", amount);
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
void Student::averageMarkMake() {
  if (marks_.empty()) {
    averageMark = 0.0;
    return;
  }

  int total = 0;

  for (const Mark &mark : marks_) {
    total += mark.value();
  }

  averageMark = static_cast<double>(total) / marks_.size();
}

void Student::addMark(const Mark &mark) {
  marks_.push_back(mark);
  averageMarkMake();
}
void Student::setSubGroup(int subGroupNumEx) {
  if (subGroupNumEx < 0) {
    throw std::invalid_argument("Subgroup number cannot be negative");
  }

  subGroupNum_ = subGroupNumEx;
}
int Student::subGroupNum() const { return subGroupNum_; }
