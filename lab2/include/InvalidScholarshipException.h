#pragma once

#include <string>

class DeanOfficeException;

class InvalidScholarshipException : public DeanOfficeException {
public:
  explicit InvalidScholarshipException(const std::string &message)
      : DeanOfficeException(message) {}
};
