#pragma once

#include "AcademicOrder.h"

#include <string>

class TransferOrder : public AcademicOrder {
public:
  TransferOrder(int number, std::string studentId, std::string fromGroup,
                std::string toGroup);
  const std::string &studentId() const;
  const std::string &fromGroup() const;
  const std::string &toGroup() const;

private:
  std::string studentId_;
  std::string fromGroup_;
  std::string toGroup_;
};
