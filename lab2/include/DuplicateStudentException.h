#pragma once

#include <string>

class DeanOfficeException;

class DuplicateStudentException : public DeanOfficeException {
public:
  explicit DuplicateStudentException(const std::string &message)
      : DeanOfficeException(message) {}
};
