#pragma once

#include <string>
#include <vector>


class Scholarship {
public:
    Scholarship(std::string name, double amount);
    void changeAmount(double amount);
    const std::string& name() const;
    double amount() const;

private:
    std::string name_;
    double amount_;
};

class ScholarshipApplication {
public:
    ScholarshipApplication(std::string studentId, std::string scholarshipName);
    void approve();
    void reject(const std::string& reason);
    bool isResolved() const;
    bool isApproved() const;
    const std::string& decisionReason() const;

private:
    std::string studentId_;
    std::string scholarshipName_;
    bool resolved_{false};
    bool approved_{false};
    std::string decisionReason_;
};

class PaymentRecord {
public:
    PaymentRecord(std::string studentId, double amount, std::string purpose);
    void markPaid();
    bool isPaid() const;
    double amount() const;
    const std::string& purpose() const;

private:
    std::string studentId_;
    double amount_;
    std::string purpose_;
    bool paid_{false};
};

class FinancialReport {
public:
    explicit FinancialReport(std::string period);
    void addIncome(double amount);
    void addExpense(double amount);
    double balance() const;
    const std::string& period() const;

private:
    std::string period_;
    double income_{0.0};
    double expenses_{0.0};
};

class Benefit {
public:
    Benefit(std::string name, std::string description);
    void assignTo(const std::string& studentId);
    bool assignedTo(const std::string& studentId) const;
    const std::string& name() const;

private:
    std::string name_;
    std::string description_;
    std::vector<std::string> studentIds_;
};

class AccountingDepartment {
public:
    explicit AccountingDepartment(std::string name);
    void recordPayment(const PaymentRecord& record);
    std::size_t paymentCount() const;
    double totalPaid() const;
    const std::string& name() const;

private:
    std::string name_;
    std::vector<PaymentRecord> payments_;
};

