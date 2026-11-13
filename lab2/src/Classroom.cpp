#include "Classroom.h"

#include <stdexcept>
#include <utility>

Classroom::Classroom(std::string roomNumber, int capacity)
    : roomNumber_(std::move(roomNumber)), capacity_(capacity) {

  if (roomNumber_.empty()) {
    throw std::invalid_argument("Room number cannot be empty");
  }

  if (capacity_ <= 0) {
    throw std::invalid_argument("Classroom capacity must be positive");
  }
}

const std::string &Classroom::roomNumber() const { return roomNumber_; }

int Classroom::capacity() const { return capacity_; }

bool Classroom::canFit(int peopleCount) const {
  if (peopleCount < 0) {
    throw std::invalid_argument("People count cannot be negative");
  }

  return peopleCount <= capacity_;
}
