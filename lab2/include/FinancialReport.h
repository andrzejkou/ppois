#pragma once

#include <string>

class FinancialReport {
public:
  explicit FinancialReport(std::string period);

  void addIncome(double amount);
  void addExpense(double amount);
  double balance() const;
  const std::string &period() const;

private:
  std::string period_;
  double income_{0.0};
  double expenses_{0.0};
};
