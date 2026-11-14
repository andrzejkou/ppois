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
      submissions_(std::move(other.submissions_)),
      applications_(std::move(other.applications_)),
      scholarshipApplications_(std::move(other.scholarshipApplications_)),
      academicDebtCount_(other.academicDebtCount_) {

  other.studentId_.clear();
  other.subGroupNum_ = 0;
  other.year_ = 0;
  other.averageMark = 0.0;
  other.academicDebtCount_ = 0;
}

double Student::averageMarkCheck() const { return averageMark; }

bool Student::hasMarkForSubject(const std::string &subjectCode) const {
  return std::any_of(marks_.begin(), marks_.end(),
                     [&subjectCode](const Mark &mark) {
                       return mark.subject().code() == subjectCode;
                     });
}

double Student::averageForSubject(const std::string &subjectCode) const {
  int total = 0;
  std::size_t count = 0;
  for (const Mark &mark : marks_) {
    if (mark.subject().code() == subjectCode) {
      total += mark.value();
      ++count;
    }
  }
  return count == 0 ? 0.0 : static_cast<double>(total) / count;
}

std::size_t Student::removeMarksForSubject(const std::string &subjectCode) {
  const auto oldSize = marks_.size();
  marks_.erase(std::remove_if(marks_.begin(), marks_.end(),
                              [&subjectCode](const Mark &mark) {
                                return mark.subject().code() == subjectCode;
                              }),
                marks_.end());
  averageMarkMake();
  return oldSize - marks_.size();
}

std::size_t Student::countMarksBelow(int threshold) const {
  if (threshold < 0 || threshold > 10) {
    throw std::invalid_argument("Grade threshold must be between 0 and 10");
  }
  return static_cast<std::size_t>(std::count_if(
      marks_.begin(), marks_.end(), [threshold](const Mark &mark) {
        return mark.value() < threshold;
      }));
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
void Student::removeScholarship() { scholarship_.reset(); }

void Student::assignBenefit(const Benefit &benefit) {
  if (benefit_) {
    throw std::invalid_argument("Student already has an assigned benefit");
  }
  Benefit assigned = benefit;
  assigned.assignTo(studentId_);
  benefit_ = std::move(assigned);
}

void Student::removeBenefit() {
  if (!benefit_) {
    throw std::invalid_argument("Student has no assigned benefit");
  }
  benefit_.reset();
}

std::size_t Student::recalculateAcademicDebtCount(int passingGrade) {
  if (passingGrade < 0 || passingGrade > 10) {
    throw std::invalid_argument("Passing grade must be between 0 and 10");
  }

  std::vector<std::string> failedSubjects;
  for (const auto &mark : marks_) {
    const auto &code = mark.subject().code();
    if (mark.value() < passingGrade &&
        std::find(failedSubjects.begin(), failedSubjects.end(), code) ==
            failedSubjects.end()) {
      failedSubjects.push_back(code);
    }
  }

  academicDebtCount_ = static_cast<int>(failedSubjects.size());
  return failedSubjects.size();
}

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

void Student::addSubmission(const Submission &submission) {
  if (submission.graded()) {
    throw std::invalid_argument("A graded submission cannot be added as new");
  }
  for (const auto &existing : submissions_) {
    if (existing.workTitle() == submission.workTitle()) {
      throw std::invalid_argument("Submission for this work already exists");
    }
  }
  submissions_.push_back(submission);
}

bool Student::recordSubmissionScore(const std::string &workTitle, int score,
                                    int maxScore) {
  for (auto &submission : submissions_) {
    if (submission.workTitle() == workTitle) {
      submission.recordScore(score, maxScore);
      return true;
    }
  }
  return false;
}

std::size_t Student::submissionCount() const { return submissions_.size(); }

const std::vector<Submission> &Student::submissions() const {
  return submissions_;
}

void Student::submitApplication(const StudentApplication &application) {
  if (application.studentId() != studentId_) {
    throw std::invalid_argument("Application belongs to another student");
  }
  for (const auto &existing : applications_) {
    if (existing.topic() == application.topic()) {
      throw std::invalid_argument("Application with this topic already exists");
    }
  }
  applications_.push_back(application);
}

bool Student::reviewApplication(const std::string &topic) {
  for (auto &application : applications_) {
    if (application.topic() == topic && !application.reviewed()) {
      application.markReviewed();
      return true;
    }
  }
  return false;
}

std::size_t Student::applicationCount() const { return applications_.size(); }

void Student::requestScholarship(const ScholarshipApplication &application) {
  if (application.studentId() != studentId_) {
    throw std::invalid_argument("Scholarship application belongs to another student");
  }
  for (const auto &existing : scholarshipApplications_) {
    if (existing.scholarshipName() == application.scholarshipName() &&
        !existing.isResolved()) {
      throw std::invalid_argument("An unresolved application already exists");
    }
  }
  scholarshipApplications_.push_back(application);
}

bool Student::resolveScholarshipApplication(
    const std::string &scholarshipName, bool approved, const std::string &reason) {
  for (auto &application : scholarshipApplications_) {
    if (application.scholarshipName() == scholarshipName &&
        !application.isResolved()) {
      if (approved) {
        application.approve();
      } else {
        application.reject(reason);
      }
      return true;
    }
  }
  return false;
}

std::size_t Student::scholarshipApplicationCount() const {
  return scholarshipApplications_.size();
}
