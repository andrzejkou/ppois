#pragma once

#include <string>

class DeanOfficeException;

class SpecialtyMismatchException : public DeanOfficeException {
public:
  explicit SpecialtyMismatchException(const std::string &message)
      : DeanOfficeException(message) {}
};
