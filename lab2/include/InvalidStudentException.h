#pragma once

#include <string>

class DeanOfficeException;

class InvalidStudentException : public DeanOfficeException {
public:
  explicit InvalidStudentException(const std::string &message)
      : DeanOfficeException(message) {}
};
