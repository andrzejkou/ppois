#pragma once

#include <string>
#include <vector>

class CashAccount;
class Payment {
public:
    Payment(std::string paymentId, std::string purpose, int quantity = 0, double amount = 0.0);

    bool processPayment() const;
    bool refund(int delta);

    const std::string& paymentId() const noexcept;
    const std::string& purpose() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool charge(CashAccount& account);
    bool isRefundable() const noexcept;
private:
    std::string paymentId_{};
    std::string purpose_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
