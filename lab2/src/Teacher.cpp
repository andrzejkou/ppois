#include "Teacher.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

Teacher::Teacher(std::string fullName, int age, std::string employeeId,
                 std::string department)
    : Worker(std::move(fullName), age, std::move(employeeId), "Преподаватель"),
      department_(std::move(department)) {

  if (department_.empty()) {
    throw std::invalid_argument("Department cannot be empty");
  }
}

void Teacher::printInfo() const {
  Worker::printInfo();

  std::cout << "Department: " << department_ << '\n';

  std::cout << "Courses:\n";
  for (const auto &course : courses_) {
    std::cout << " - " << course << '\n';
  }
}

const std::string &Teacher::department() const { return department_; }

void Teacher::assignCourse(const std::string &courseName) {
  if (courseName.empty()) {
    throw std::invalid_argument("Course name cannot be empty");
  }

  // Проверяем, не назначен ли уже такой курс.
  auto it = std::find(courses_.begin(), courses_.end(), courseName);

  if (it != courses_.end()) {
    throw std::invalid_argument("Course is already assigned");
  }

  courses_.push_back(courseName);
}

bool Teacher::removeCourse(const std::string &courseName) {
  const auto it = std::find(courses_.begin(), courses_.end(), courseName);
  if (it == courses_.end()) {
    return false;
  }
  courses_.erase(it);
  return true;
}

bool Teacher::replaceCourse(const std::string &oldName,
                            const std::string &newName) {
  if (newName.empty() || oldName == newName ||
      std::find(courses_.begin(), courses_.end(), newName) != courses_.end()) {
    return false;
  }
  const auto it = std::find(courses_.begin(), courses_.end(), oldName);
  if (it == courses_.end()) {
    return false;
  }
  *it = newName;
  return true;
}

void Teacher::clearTeachingAssignments() {
  courses_.clear();
  subjects_.clear();
}

const std::vector<std::string> &Teacher::courses() const { return courses_; }

std::string Teacher::role() const { return "Преподаватель"; }

void Teacher::assignSubject(const Subject &subject) {
  if (teachesSubject(subject.code())) {
    throw std::invalid_argument("Subject is already assigned");
  }
  subjects_.push_back(subject);
}

bool Teacher::removeSubject(const std::string &subjectCode) {
  auto it = std::find_if(subjects_.begin(), subjects_.end(),
                         [&subjectCode](const Subject &subject) {
                           return subject.code() == subjectCode;
                         });
  if (it == subjects_.end()) {
    return false;
  }
  subjects_.erase(it);
  return true;
}

bool Teacher::teachesSubject(const std::string &subjectCode) const {
  return std::any_of(subjects_.begin(), subjects_.end(),
                     [&subjectCode](const Subject &subject) {
                       return subject.code() == subjectCode;
                     });
}

std::size_t Teacher::subjectCount() const { return subjects_.size(); }

const std::vector<Subject> &Teacher::subjects() const { return subjects_; }
