#pragma once

#include <string>
#include <vector>

class StockAlert {
public:
    StockAlert(std::string alertId, std::string productSku, int quantity = 0, double amount = 0.0);

    bool triggerIfLow() const;
    bool acknowledge(int delta);

    const std::string& alertId() const noexcept;
    const std::string& productSku() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string alertId_{};
    std::string productSku_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
