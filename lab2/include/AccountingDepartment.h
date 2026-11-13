#pragma once

#include "FinancialReport.h"
#include "PaymentRecord.h"

#include <string>
#include <vector>
#include <cstddef>

class AccountingDepartment {
public:
  explicit AccountingDepartment(std::string name);

  FinancialReport makeReport(const std::string &period) const;
  void recordPayment(const PaymentRecord &record);
  std::size_t paymentCount() const;
  double totalPaid() const;
  const std::string &name() const;

private:
  std::string name_;
  std::vector<PaymentRecord> payments_;
};
