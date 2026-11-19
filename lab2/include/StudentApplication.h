#pragma once

#include <string>

class StudentApplication {
public:
  StudentApplication(std::string studentId, std::string topic);
  void markReviewed();
  bool reviewed() const;
  const std::string &studentId() const;
  const std::string &topic() const;

private:
  std::string studentId_;
  std::string topic_;
  bool reviewed_{false};
};
