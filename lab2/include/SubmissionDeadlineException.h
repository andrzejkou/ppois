#pragma once

#include <string>

class DeanOfficeException;

class SubmissionDeadlineException : public DeanOfficeException {
public:
  explicit SubmissionDeadlineException(const std::string &message)
      : DeanOfficeException(message) {}
};
