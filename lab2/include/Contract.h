#pragma once

#include "Company.h"

#include <vector>

class Contract {
public:
  void addCompany(const Company &company);
  void makeContract();
  bool isMade() const;

private:
  std::vector<Company> company;
  bool made_{false};
};
