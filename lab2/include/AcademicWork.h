#pragma once

#include <utility>

#include <string>

class AcademicWork {
public:
  AcademicWork(std::string title, int maxScore);
  virtual ~AcademicWork() = default;
  virtual std::string type() const;
  const std::string &title() const;
  int maxScore() const;
  const std::string &description() const { return description_; }
  void setDescription(std::string value) { description_ = std::move(value); }
  const std::string &deadline() const { return deadline_; }
  void setDeadline(std::string value) { deadline_ = std::move(value); }

private:
  std::string title_;
  int maxScore_;
  std::string description_;
  std::string deadline_;
};
