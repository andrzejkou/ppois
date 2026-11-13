#pragma once

#include <string>

class DeanOfficeException;

class GroupCapacityException : public DeanOfficeException {
public:
  explicit GroupCapacityException(const std::string &message)
      : DeanOfficeException(message) {}
};
