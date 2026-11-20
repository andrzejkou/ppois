#pragma once

#include <string>
#include <vector>

class Payment;
class CashAccount {
public:
    CashAccount(std::string accountId, std::string currency, int quantity = 0, double amount = 0.0);

    bool deposit() const;
    bool withdraw(int delta);

    const std::string& accountId() const noexcept;
    const std::string& currency() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool recordPayment(const Payment& payment);
    bool credit(int amount);
    bool creditMoney(double amount);
    bool withdrawMoney(double amount);
private:
    std::string accountId_{};
    std::string currency_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
