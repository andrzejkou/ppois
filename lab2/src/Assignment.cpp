#include "Assignment.h"

#include <stdexcept>
#include <utility>

Assignment::Assignment(std::string studentId, std::string workTitle)
    : studentId_(std::move(studentId)),
      workTitle_(std::move(workTitle)) {
  if (studentId_.empty()) {
    throw std::invalid_argument("Student ID cannot be empty");
  }

  if (workTitle_.empty()) {
    throw std::invalid_argument("Work title cannot be empty");
  }
}

void Assignment::submit(const std::string &fileName) {
  if (fileName.empty()) {
    throw std::invalid_argument("Submission file name cannot be empty");
  }

  fileName_ = fileName;
}

bool Assignment::isSubmitted() const {
  return !fileName_.empty();
}

const std::string &Assignment::fileName() const {
  return fileName_;
}
