#pragma once

#include "AcademicOrder.h"

#include <string>

class ExpulsionOrder : public AcademicOrder {
public:
  ExpulsionOrder(int number, std::string studentId, std::string reason);
  const std::string &studentId() const;
  const std::string &reason() const;

private:
  std::string studentId_;
  std::string reason_;
};
