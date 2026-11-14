#include "PaymentRecord.h"

#include <stdexcept>
#include <utility>

PaymentRecord::PaymentRecord(std::string studentId, double amount,
                             std::string purpose, std::string date)
    : studentId_(std::move(studentId)), amount_(amount),
      purpose_(std::move(purpose)), date_(std::move(date)) {

  if (studentId_.empty()) {
    throw std::invalid_argument("Student ID cannot be empty");
  }

  if (amount_ <= 0) {
    throw std::invalid_argument("Payment amount must be positive");
  }

  if (purpose_.empty()) {
    throw std::invalid_argument("Payment purpose cannot be empty");
  }

  if (date_.empty()) {
    throw std::invalid_argument("Payment date cannot be empty");
  }
}

void PaymentRecord::markPaid() { paid_ = true; }

bool PaymentRecord::isPaid() const { return paid_; }

double PaymentRecord::amount() const { return amount_; }

const std::string &PaymentRecord::purpose() const { return purpose_; }

const std::string &PaymentRecord::date() const { return date_; }

const std::string &PaymentRecord::studentId() const { return studentId_; }
