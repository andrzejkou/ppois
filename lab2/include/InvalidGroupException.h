#pragma once

#include <string>

class DeanOfficeException;

class InvalidGroupException : public DeanOfficeException {
public:
  explicit InvalidGroupException(const std::string &message)
      : DeanOfficeException(message) {}
};
