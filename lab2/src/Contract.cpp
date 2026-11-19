#include "Contract.h"

#include <algorithm>
#include <stdexcept>

void Contract::addCompany(const Company &companyToAdd) {
  const auto it = std::find_if(
      company.begin(), company.end(),
      [&companyToAdd](const Company &existing) {
        return existing.name() == companyToAdd.name();
      });

  if (it != company.end()) {
    throw std::invalid_argument("Company is already in the contract");
  }

  company.push_back(companyToAdd);
}

void Contract::makeContract() {
  if (company.size() < 2) {
    throw std::logic_error(
        "A contract requires at least two companies");
  }

  made_ = true;
}

bool Contract::isMade() const {
  return made_;
}
