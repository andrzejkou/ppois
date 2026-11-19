#pragma once

#include <string>

class Evaluation {
public:
  Evaluation(std::string evaluatorId, std::string comment);
  void reviseComment(const std::string &comment);
  const std::string &evaluatorId() const;
  const std::string &comment() const;

private:
  std::string evaluatorId_;
  std::string comment_;
};
