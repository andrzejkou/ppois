#pragma once

#include <string>
#include <vector>

class StockItem;
class StockMovement {
public:
    StockMovement(std::string movementId, std::string productSku, int quantity = 0, double amount = 0.0);

    double calculateSignedQuantity() const;
    bool isTransfer(int delta);

    const std::string& movementId() const noexcept;
    const std::string& productSku() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool tracks(const StockItem& item) const;
    bool isInbound() const noexcept;
private:
    std::string movementId_{};
    std::string productSku_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
