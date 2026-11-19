#pragma once

#include <string>
#include <vector>

class Inventory {
public:
    Inventory(std::string inventoryId, std::string warehouseCode, int quantity = 0, double amount = 0.0);

    bool startCounting() const;
    bool finishCounting(int delta);

    const std::string& inventoryId() const noexcept;
    const std::string& warehouseCode() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string inventoryId_{};
    std::string warehouseCode_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
