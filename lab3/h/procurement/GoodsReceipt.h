#pragma once

#include <string>
#include <vector>

class GoodsReceipt {
public:
    GoodsReceipt(std::string receiptId, std::string purchaseOrderId, int quantity = 0, double amount = 0.0);

    bool acceptDelivery() const;
    bool calculateAcceptedValue(int delta);

    const std::string& receiptId() const noexcept;
    const std::string& purchaseOrderId() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string receiptId_{};
    std::string purchaseOrderId_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
