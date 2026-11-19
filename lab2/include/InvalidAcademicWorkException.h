#pragma once

#include <string>

class DeanOfficeException;

class InvalidAcademicWorkException : public DeanOfficeException {
public:
  explicit InvalidAcademicWorkException(const std::string &message)
      : DeanOfficeException(message) {}
};
