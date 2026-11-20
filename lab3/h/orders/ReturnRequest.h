#pragma once

#include <string>
#include <vector>

class ReturnRequest {
public:
    ReturnRequest(std::string returnId, std::string orderId, int quantity = 0, double amount = 0.0);

    bool approveReturn() const;
    bool calculateRefund(int delta);

    const std::string& returnId() const noexcept;
    const std::string& orderId() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool isWithinQuantityLimit(int maximumQuantity) const noexcept;
private:
    std::string returnId_{};
    std::string orderId_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
