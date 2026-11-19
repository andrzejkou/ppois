#pragma once

#include <string>
#include <vector>

class StockAdjustment {
public:
    StockAdjustment(std::string adjustmentId, std::string reason, int quantity = 0, double amount = 0.0);

    bool applyAdjustment() const;
    bool requiresApproval(int delta);

    const std::string& adjustmentId() const noexcept;
    const std::string& reason() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string adjustmentId_{};
    std::string reason_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
