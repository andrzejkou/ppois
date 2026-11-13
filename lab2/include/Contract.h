#pragma once

#include "Company.h"

#include <vector>

class Contract {
public:
  void makeContract();

private:
  std::vector<Company> company;
};
