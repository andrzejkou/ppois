#pragma once

#include <string>
#include <vector>

class StockItem {
public:
    StockItem(std::string stockId, std::string productSku, int quantity = 0, double amount = 0.0);

    bool increaseStock() const;
    bool decreaseStock(int delta);

    const std::string& stockId() const noexcept;
    const std::string& productSku() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string stockId_{};
    std::string productSku_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
