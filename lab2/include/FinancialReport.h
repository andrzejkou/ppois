#pragma once

#include <utility>

#include <string>

class FinancialReport {
public:
  explicit FinancialReport(std::string period);

  void addIncome(double amount);
  void addExpense(double amount);
  double balance() const;
  const std::string &period() const;
  const std::string &periodStart() const { return periodStart_; }
  void setPeriodStart(std::string value) { periodStart_ = std::move(value); }
  const std::string &periodEnd() const { return periodEnd_; }
  void setPeriodEnd(std::string value) { periodEnd_ = std::move(value); }

private:
  std::string period_;
  double income_{0.0};
  double expenses_{0.0};
  std::string periodStart_;
  std::string periodEnd_;
};
