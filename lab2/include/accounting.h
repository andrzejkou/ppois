#pragma once

#include <string>
#include <vector>

/**
 * @file accounting.h
 * @brief Финансовый учёт деканата
 */

class Premy {
public:
  Premy(std::string name, double amount);
  virtual ~Premy() = default;

  virtual void changeAmount(double amount);
  const std::string &name() const;
  double amount() const;

private:
  std::string name_;
  double amount_;
};

class Scholarship : public Premy {
public:
  Scholarship(std::string name, double amount);
  void changeAmount(double amount) override;
};

class ScholarshipApplication {
public:
  ScholarshipApplication(std::string studentId, std::string scholarshipName);

  void approve();
  void reject(const std::string &reason);
  bool isResolved() const;
  bool isApproved() const;
  const std::string &decisionReason() const;

private:
  std::string studentId_;
  std::string scholarshipName_;
  bool resolved_{false};
  bool approved_{false};
  std::string decisionReason_;
};

class PaymentRecord {
public:
  PaymentRecord(std::string studentId, double amount, std::string purpose,
                std::string date);

  void markPaid();
  bool isPaid() const;
  double amount() const;
  const std::string &purpose() const;
  const std::string &date() const;

private:
  std::string studentId_;
  double amount_;
  std::string purpose_;
  std::string date_;
  bool paid_{false};
};

class FinancialReport {
public:
  explicit FinancialReport(std::string period);

  void addIncome(double amount);
  void addExpense(double amount);
  double balance() const;
  const std::string &period() const;

private:
  std::string period_;
  double income_{0.0};
  double expenses_{0.0};
};

class Benefit : public Premy {
public:
  Benefit(std::string name, double amount, std::string description);

  void assignTo(const std::string &studentId);
  bool assignedTo(const std::string &studentId) const;

private:
  std::string description_;
  std::vector<std::string> studentIds_;
};

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
