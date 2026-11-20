#include "AccountingDepartment.h"
#include "Student.h"
#include <algorithm>
#include <array>
#include <cctype>
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

  // Supported calendar periods: YYYY, YYYY-MM, YYYY-MM-DD, or an
  // English month name (for example, "November"). Older callers also pass
  // descriptive labels; those retain the historical all-period report.
  static const std::array<const char *, 12> monthNames = {
      "january", "february", "march", "april", "may", "june",
      "july", "august", "september", "october", "november", "december"};

  std::string normalizedPeriod = period;
  std::transform(normalizedPeriod.begin(), normalizedPeriod.end(),
                 normalizedPeriod.begin(), [](unsigned char ch) {
                   return static_cast<char>(std::tolower(ch));
                 });

  bool calendarPeriod = false;
  std::string monthNumber;
  if (period.size() == 4 &&
      std::all_of(period.begin(), period.end(), [](unsigned char ch) {
        return std::isdigit(ch);
      })) {
    calendarPeriod = true;
  } else if ((period.size() == 7 || period.size() == 10) &&
             period[4] == '-') {
    const bool validYear = std::all_of(period.begin(), period.begin() + 4,
        [](unsigned char ch) { return std::isdigit(ch); });
    const bool validMonth = std::isdigit(period[5]) && std::isdigit(period[6]) &&
        period.substr(5, 2) >= "01" && period.substr(5, 2) <= "12";
    const bool validDay = period.size() == 7 ||
        (period[7] == '-' && std::isdigit(period[8]) && std::isdigit(period[9]) &&
         period.substr(8, 2) >= "01" && period.substr(8, 2) <= "31");
    calendarPeriod = validYear && validMonth && validDay;
  } else {
    for (std::size_t i = 0; i < monthNames.size(); ++i) {
      if (normalizedPeriod == monthNames[i]) {
        calendarPeriod = true;
        monthNumber = (i + 1 < 10 ? "0" : "") + std::to_string(i + 1);
        break;
      }
    }
  }

  for (const auto &payment : payments_) {
    const std::string &paymentDate = payment.date();
    bool matchesPeriod = true;
    if (calendarPeriod) {
      if (!monthNumber.empty()) {
        matchesPeriod = paymentDate.size() >= 7 &&
                        paymentDate.compare(5, 2, monthNumber) == 0;
      } else {
        matchesPeriod = paymentDate.size() >= period.size() &&
                        paymentDate.compare(0, period.size(), period) == 0;
      }
    }

    if (payment.isPaid() && matchesPeriod) {
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

std::size_t AccountingDepartment::cancelUnpaidPaymentsForPurpose(
    const std::string &studentId, const std::string &purpose) {
  const auto oldSize = payments_.size();
  payments_.erase(std::remove_if(payments_.begin(), payments_.end(),
      [&studentId, &purpose](const PaymentRecord &payment) {
        return payment.studentId() == studentId &&
               payment.purpose() == purpose && !payment.isPaid();
      }), payments_.end());
  return oldSize - payments_.size();
}

std::size_t AccountingDepartment::settleUnpaidPaymentsForPurpose(
    const std::string &studentId, const std::string &purpose) {
  std::size_t settled = 0;
  for (auto &payment : payments_) {
    if (payment.studentId() == studentId && payment.purpose() == purpose &&
        !payment.isPaid()) {
      payment.markPaid();
      ++settled;
    }
  }
  return settled;
}

double AccountingDepartment::totalForPurpose(const std::string &purpose,
                                             bool paidOnly) const {
  double total = 0.0;
  for (const auto &payment : payments_) {
    if (payment.purpose() == purpose && (!paidOnly || payment.isPaid())) {
      total += payment.amount();
    }
  }
  return total;
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
