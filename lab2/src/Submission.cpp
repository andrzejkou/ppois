#include "Submission.h"

#include <stdexcept>
#include <utility>

Submission::Submission(std::string studentId, std::string workTitle)
    : studentId_(std::move(studentId)),
      workTitle_(std::move(workTitle)) {
  if (studentId_.empty()) {
    throw std::invalid_argument("Student ID cannot be empty");
  }

  if (workTitle_.empty()) {
    throw std::invalid_argument("Work title cannot be empty");
  }
}

void Submission::recordScore(int score, int maxScore) {
  if (maxScore <= 0) {
    throw std::invalid_argument("Maximum score must be positive");
  }

  if (score < 0 || score > maxScore) {
    throw std::out_of_range("Score is outside the allowed range");
  }

  score_ = score;
}

bool Submission::graded() const {
  return score_ >= 0;
}

int Submission::score() const {
  return score_;
}

const std::string &Submission::workTitle() const { return workTitle_; }

const std::string &Submission::studentId() const { return studentId_; }
