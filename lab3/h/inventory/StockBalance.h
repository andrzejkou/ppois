#pragma once

#include <string>
#include <vector>

class Warehouse;
class StockBalance {
public:
    StockBalance(std::string balanceId, std::string warehouseCode, int quantity = 0, double amount = 0.0);

    double calculateAvailable() const;
    bool reconcile(int delta);

    const std::string& balanceId() const noexcept;
    const std::string& warehouseCode() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool belongsTo(const Warehouse& warehouse) const;
    bool hasAtLeast(int requested) const noexcept;
private:
    std::string balanceId_{};
    std::string warehouseCode_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
