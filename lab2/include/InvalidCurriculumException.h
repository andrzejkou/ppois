#pragma once

#include <string>

class DeanOfficeException;

class InvalidCurriculumException : public DeanOfficeException {
public:
  explicit InvalidCurriculumException(const std::string &message)
      : DeanOfficeException(message) {}
};
