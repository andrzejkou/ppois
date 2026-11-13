
#include "Worker.h"

#include <iostream>
#include <stdexcept>
#include <utility>

Worker::Worker(std::string fullName, int age, std::string employeeId,
               std::string position)
    : People(std::move(fullName), age), employeeId_(std::move(employeeId)),
      position_(std::move(position)) {

  if (employeeId_.empty()) {
    throw std::invalid_argument("Employee ID cannot be empty");
  }

  if (position_.empty()) {
    throw std::invalid_argument("Position cannot be empty");
  }
}

void Worker::printInfo() const {
  People::printInfo();

  std::cout << "Employee ID: " << employeeId_ << '\n'
            << "Position: " << position_ << '\n';
}

const std::string &Worker::employeeId() const { return employeeId_; }

const std::string &Worker::position() const { return position_; }

void Worker::changePosition(const std::string &newPosition) {
  if (newPosition.empty()) {
    throw std::invalid_argument("Position cannot be empty");
  }

  position_ = newPosition;
}

std::string Worker::role() const { return "Сотрудник"; }
