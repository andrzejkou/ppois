#pragma once

#include "FinancialReport.h"
#include "PaymentRecord.h"

class Student;

#include <string>
#include <vector>
#include <cstddef>

class AccountingDepartment {
public:
  explicit AccountingDepartment(std::string name);

  FinancialReport makeReport(const std::string &period) const;
  void recordPayment(const PaymentRecord &record);
  void recordStudentPayment(const Student &student, double amount,
                            const std::string &purpose,
                            const std::string &date);
  double totalPaidByStudent(const Student &student) const;
  std::size_t paymentCountForStudent(const Student &student) const;
  std::size_t paymentCount() const;
  double totalPaid() const;
  const std::string &name() const;

private:
  std::string name_;
  std::vector<PaymentRecord> payments_;
};
