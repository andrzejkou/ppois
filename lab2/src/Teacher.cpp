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

const std::vector<std::string> &Teacher::courses() const { return courses_; }

std::string Teacher::role() const { return "Преподаватель"; }
