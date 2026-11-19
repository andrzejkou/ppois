#pragma once

#include "AcademicWork.h"

#include <string>

class LaboratoryWork : public AcademicWork {
public:
  LaboratoryWork(std::string title, int maxScore, int number);
  int number() const;
  std::string type() const override;

private:
  int number_;
};
