#pragma once

#include <string>

class DeanOfficeException;

class InvalidPersonException : public DeanOfficeException {
public:
  explicit InvalidPersonException(const std::string &message)
      : DeanOfficeException(message) {}
};
