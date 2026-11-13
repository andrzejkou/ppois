#pragma once

#include <string>

class AcademicWork {
public:
  AcademicWork(std::string title, int maxScore);
  virtual ~AcademicWork() = default;
  virtual std::string type() const;
  const std::string &title() const;
  int maxScore() const;

private:
  std::string title_;
  int maxScore_;
};
