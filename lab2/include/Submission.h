#pragma once

#include <string>

class Submission {
public:
  Submission(std::string studentId, std::string workTitle);
  void recordScore(int score, int maxScore);
  bool graded() const;
  int score() const;
  const std::string &workTitle() const;
  const std::string &studentId() const;

private:
  std::string studentId_;
  std::string workTitle_;
  int score_{-1};
};
