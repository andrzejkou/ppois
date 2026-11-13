#pragma once

#include "Worker.h"

#include <string>

class Secretary : public Worker {
public:
  void printInfo() const override;
  Secretary(std::string fullName, int age, std::string employeeId);
  std::string registerDocument(const std::string &title) const;
};
