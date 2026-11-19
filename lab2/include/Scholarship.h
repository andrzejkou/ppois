#pragma once

#include "Premy.h"

#include <string>

class Scholarship : public Premy {
public:
  Scholarship(std::string name, double amount);
  void changeAmount(double amount) override;
};
