#pragma once

#include <string>

class PaymentRecord {
public:
  PaymentRecord(std::string studentId, double amount, std::string purpose,
                std::string date);

  void markPaid();
  bool isPaid() const;
  double amount() const;
  const std::string &purpose() const;
  const std::string &date() const;
  const std::string &studentId() const;

private:
  std::string studentId_;
  double amount_;
  std::string purpose_;
  std::string date_;
  bool paid_{false};
};
