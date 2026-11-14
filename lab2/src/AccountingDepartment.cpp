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

double AccountingDepartment::totalPaidForStudent(const std::string &studentId) const {
  double total = 0.0;
  for (const auto &payment : payments_)
    if (payment.studentId() == studentId && payment.isPaid()) total += payment.amount();
  return total;
}

double AccountingDepartment::totalUnpaid() const {
  double total = 0.0;
  for (const auto &payment : payments_)
    if (!payment.isPaid()) total += payment.amount();
  return total;
}

std::size_t AccountingDepartment::markStudentPaymentsPaid(const std::string &studentId) {
  std::size_t changed = 0;
  for (auto &payment : payments_) {
    if (payment.studentId() == studentId && !payment.isPaid()) {
      payment.markPaid();
      ++changed;
    }
  }
  return changed;
}

std::size_t AccountingDepartment::removeUnpaidPayments(const std::string &studentId) {
  const auto oldSize = payments_.size();
  payments_.erase(std::remove_if(payments_.begin(), payments_.end(),
      [&studentId](const PaymentRecord &payment) {
        return payment.studentId() == studentId && !payment.isPaid();
      }), payments_.end());
  return oldSize - payments_.size();
}

bool AccountingDepartment::hasPayment(const std::string &studentId,
                                      const std::string &purpose) const {
  return std::any_of(payments_.begin(), payments_.end(),
      [&studentId, &purpose](const PaymentRecord &payment) {
        return payment.studentId() == studentId && payment.purpose() == purpose;
      });
}

std::vector<PaymentRecord> AccountingDepartment::paymentsForStudent(
    const std::string &studentId) const {
  std::vector<PaymentRecord> result;
  for (const auto &payment : payments_)
    if (payment.studentId() == studentId) result.push_back(payment);
  return result;
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
