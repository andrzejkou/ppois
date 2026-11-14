#include "AccountingDepartment.h"
#include "Student.h"
#include <algorithm>
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

void AccountingDepartment::recordStudentPayment(
    const Student &student, double amount, const std::string &purpose,
    const std::string &date) {
  recordPayment(PaymentRecord(student.studentId(), amount, purpose, date));
}

double AccountingDepartment::totalPaidByStudent(const Student &student) const {
  double total = 0.0;
  for (const auto &payment : payments_) {
    if (payment.studentId() == student.studentId() && payment.isPaid()) {
      total += payment.amount();
    }
  }
  return total;
}

std::size_t AccountingDepartment::paymentCountForStudent(
    const Student &student) const {
  return static_cast<std::size_t>(std::count_if(
      payments_.begin(), payments_.end(),
      [&student](const PaymentRecord &payment) {
        return payment.studentId() == student.studentId();
      }));
}
