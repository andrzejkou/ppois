#pragma once

#include "People.h"

#include <string>

class Worker : public People {
public:
  Worker(std::string fullName, int age, std::string employeeId,
         std::string position);
  void printInfo() const override;

  const std::string &employeeId() const;
  const std::string &position() const;
  void changePosition(const std::string &position);
  std::string role() const override;

protected:
  std::string employeeId_;
  std::string position_;
};
