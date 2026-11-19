#pragma once

#include <utility>

#include <string>

class Submission {
public:
  Submission(std::string studentId, std::string workTitle);
  void recordScore(int score, int maxScore);
  bool graded() const;
  int score() const;
  const std::string &workTitle() const;
  const std::string &studentId() const;
  const std::string &submittedAt() const { return submittedAt_; }
  void setSubmittedAt(std::string value) { submittedAt_ = std::move(value); }
  const std::string &gradedAt() const { return gradedAt_; }
  void setGradedAt(std::string value) { gradedAt_ = std::move(value); }
  const std::string &feedback() const { return feedback_; }
  void setFeedback(std::string value) { feedback_ = std::move(value); }

private:
  std::string studentId_;
  std::string workTitle_;
  int score_{-1};
  std::string submittedAt_;
  std::string gradedAt_;
  std::string feedback_;
};
