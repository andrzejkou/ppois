#pragma once

#include "Worker.h"

#include <string>

class AcademicOrder;

class Dean : public Worker {
public:
  void printInfo() const override;

  Dean(std::string fullName, int age, std::string employeeId,
       std::string position);
  void approveOrder(AcademicOrder &order) const;

  std::string role() const override;
};
