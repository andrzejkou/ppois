#pragma once

#include <string>
#include <stdexcept>

class DeanOfficeException : public std::runtime_error {
public:
  explicit DeanOfficeException(const std::string &message)
      : std::runtime_error(message) {}
};
