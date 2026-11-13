#include "AccountingDepartment.h"
#include <stdexcept>
#include <utility>

AccountingDepartment::AccountingDepartment(std::string name)
    : name_(std::move(name)) {

  if (name_.empty()) {
    throw std::invalid_argument("Accounting department name cannot be empty");
  }
}

FinancialReport
AccountingDepartment::makeReport(const std::string &period) const {

  FinancialReport report(period);

  for (const auto &payment : payments_) {
    if (payment.isPaid()) {
      report.addExpense(payment.amount());
    }
  }

  return report;
}

void AccountingDepartment::recordPayment(const PaymentRecord &record) {
  payments_.push_back(record);
}

std::size_t AccountingDepartment::paymentCount() const {
  return payments_.size();
}

double AccountingDepartment::totalPaid() const {
  double total = 0.0;

  for (const auto &payment : payments_) {
    if (payment.isPaid()) {
      total += payment.amount();
    }
  }

  return total;
}

const std::string &AccountingDepartment::name() const { return name_; }
