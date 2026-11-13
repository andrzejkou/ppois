#pragma once

#include <string>

class DeanOfficeException;

class FinancialOperationException : public DeanOfficeException {
public:
  explicit FinancialOperationException(const std::string &message)
      : DeanOfficeException(message) {}
};
