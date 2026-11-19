#pragma once

#include <string>

class DeanOfficeException;

class ClassroomUnavailableException : public DeanOfficeException {
public:
  explicit ClassroomUnavailableException(const std::string &message)
      : DeanOfficeException(message) {}
};
