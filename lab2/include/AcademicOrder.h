#pragma once

#include <utility>

#include <string>

class Dean;

class AcademicOrder {
public:
  AcademicOrder(int number, std::string title);
  void approve(const Dean &dean);
  bool isApproved() const;
  int number() const;
  const std::string &title() const;
  const std::string &issueDate() const { return issueDate_; }
  void setIssueDate(std::string value) { issueDate_ = std::move(value); }
  const std::string &effectiveDate() const { return effectiveDate_; }
  void setEffectiveDate(std::string value) { effectiveDate_ = std::move(value); }
  const std::string &approverEmployeeId() const { return approverEmployeeId_; }
  void setApproverEmployeeId(std::string value) { approverEmployeeId_ = std::move(value); }

private:
  int number_;
  std::string title_;
  bool approved_{false};
  std::string issueDate_;
  std::string effectiveDate_;
  std::string approverEmployeeId_;
};
