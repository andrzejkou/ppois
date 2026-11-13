#pragma once

#include <string>

class DeanOfficeException;

class InvalidScheduleException : public DeanOfficeException {
public:
  explicit InvalidScheduleException(const std::string &message)
      : DeanOfficeException(message) {}
};
