#pragma once

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class Company {
public:
  Company(std::string typeOfContract, std::string name);
  const std::string &contractType() const;
  const std::string &name() const;

  void addPartner(const std::shared_ptr<Company> &company);
  bool removePartner(const std::string &companyName);
  bool hasPartner(const std::string &companyName) const;
  void printInfo() const;

private:
  std::string typeOfContract_;
  std::string name_;
  std::vector<std::shared_ptr<Company>> integrCompanies_;
};
