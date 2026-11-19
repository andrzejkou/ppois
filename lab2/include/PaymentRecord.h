#pragma once

#include <utility>

#include <string>

class PaymentRecord {
public:
  PaymentRecord(std::string studentId, double amount, std::string purpose,
                std::string date);

  void markPaid();
  bool isPaid() const;
  const std::string &studentId() const;
  void changeAmount(double amount);
  void changePurpose(const std::string &purpose);
  double amount() const;
  const std::string &purpose() const;
  const std::string &date() const;
  const std::string &dueDate() const { return dueDate_; }
  void setDueDate(std::string value) { dueDate_ = std::move(value); }
  const std::string &paymentMethod() const { return paymentMethod_; }
  void setPaymentMethod(std::string value) { paymentMethod_ = std::move(value); }
  const std::string &receiptNumber() const { return receiptNumber_; }
  void setReceiptNumber(std::string value) { receiptNumber_ = std::move(value); }

private:
  std::string studentId_;
  double amount_;
  std::string purpose_;
  std::string date_;
  bool paid_{false};
  std::string dueDate_;
  std::string paymentMethod_;
  std::string receiptNumber_;
};
