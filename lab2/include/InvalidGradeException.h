#pragma once

#include <string>

class DeanOfficeException;

class InvalidGradeException : public DeanOfficeException {
public:
  explicit InvalidGradeException(const std::string &message)
      : DeanOfficeException(message) {}
};
