#pragma once

#include <string>

class ScholarshipApplication {
public:
  ScholarshipApplication(std::string studentId, std::string scholarshipName);

  void approve();
  void reject(const std::string &reason);
  bool isResolved() const;
  bool isApproved() const;
  const std::string &decisionReason() const;
  const std::string &studentId() const;
  const std::string &scholarshipName() const;

private:
  std::string studentId_;
  std::string scholarshipName_;
  bool resolved_{false};
  bool approved_{false};
  std::string decisionReason_;
};
