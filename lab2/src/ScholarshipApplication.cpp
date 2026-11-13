#include "ScholarshipApplication.h"

#include <stdexcept>
#include <utility>

ScholarshipApplication::ScholarshipApplication(std::string studentId,
                                               std::string scholarshipName)
    : studentId_(std::move(studentId)),
      scholarshipName_(std::move(scholarshipName)) {

  if (studentId_.empty()) {
    throw std::invalid_argument("Student ID cannot be empty");
  }

  if (scholarshipName_.empty()) {
    throw std::invalid_argument("Scholarship name cannot be empty");
  }
}

void ScholarshipApplication::approve() {
  if (resolved_) {
    throw std::logic_error("Application has already been resolved");
  }

  resolved_ = true;
  approved_ = true;
  decisionReason_ = "Заявление одобрено";
}

void ScholarshipApplication::reject(const std::string &reason) {
  if (resolved_) {
    throw std::logic_error("Application has already been resolved");
  }

  if (reason.empty()) {
    throw std::invalid_argument("Rejection reason cannot be empty");
  }

  resolved_ = true;
  approved_ = false;
  decisionReason_ = reason;
}

bool ScholarshipApplication::isResolved() const { return resolved_; }

bool ScholarshipApplication::isApproved() const { return approved_; }

const std::string &ScholarshipApplication::decisionReason() const {
  return decisionReason_;
}
