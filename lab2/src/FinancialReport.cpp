#include "FinancialReport.h"

#include <cmath>
#include <stdexcept>
#include <utility>

FinancialReport::FinancialReport(std::string period)
    : period_(std::move(period)) {

  if (period_.empty()) {
    throw std::invalid_argument("Report period cannot be empty");
  }
}

void FinancialReport::addIncome(double amount) {
  if (!std::isfinite(amount) || amount <= 0) {
    throw std::invalid_argument("Income amount must be positive and finite");
  }

  income_ += amount;
}

void FinancialReport::addExpense(double amount) {
  if (!std::isfinite(amount) || amount <= 0) {
    throw std::invalid_argument("Expense amount must be positive and finite");
  }

  expenses_ += amount;
}

double FinancialReport::balance() const { return income_ - expenses_; }

const std::string &FinancialReport::period() const { return period_; }
