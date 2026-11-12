#include "people.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

Company::Company(std::string typeOfContract, std::string name)
    : typeOfContract_(std::move(typeOfContract)), name_(std::move(name)) {

  if (typeOfContract_.empty()) {
    throw std::invalid_argument("Contract type cannot be empty");
  }

  if (name_.empty()) {
    throw std::invalid_argument("Company name cannot be empty");
  }
}

const std::string &Company::contractType() const { return typeOfContract_; }

const std::string &Company::name() const { return name_; }

void Company::addPartner(const std::shared_ptr<Company> &company) {
  if (!company) {
    throw std::invalid_argument("Company cannot be null");
  }

  if (company.get() == this) {
    throw std::invalid_argument("Company cannot be its own partner");
  }

  if (hasPartner(company->name())) {
    throw std::invalid_argument("Partner already exists");
  }

  integrCompanies_.push_back(company);
}

bool Company::removePartner(const std::string &companyName) {
  auto it = std::find_if(integrCompanies_.begin(), integrCompanies_.end(),
                         [&](const std::shared_ptr<Company> &company) {
                           return company->name() == companyName;
                         });

  if (it == integrCompanies_.end()) {
    return false;
  }

  integrCompanies_.erase(it);
  return true;
}

bool Company::hasPartner(const std::string &companyName) const {
  return std::any_of(integrCompanies_.begin(), integrCompanies_.end(),
                     [&](const std::shared_ptr<Company> &company) {
                       return company->name() == companyName;
                     });
}

void Company::printInfo() const {
  std::cout << "Company: " << name_ << '\n'
            << "Contract type: " << typeOfContract_ << '\n'
            << "Partners:\n";

  for (const auto &company : integrCompanies_) {
    std::cout << " - " << company->name() << '\n';
  }
}
