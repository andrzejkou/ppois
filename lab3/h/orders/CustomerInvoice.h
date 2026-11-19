#pragma once

#include <string>
#include <vector>

class CustomerInvoice {
public:
    CustomerInvoice(std::string invoiceId, std::string customerId, int quantity = 0, double amount = 0.0);

    double calculateDue() const;
    bool markPaid(int delta);

    const std::string& invoiceId() const noexcept;
    const std::string& customerId() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string invoiceId_{};
    std::string customerId_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
