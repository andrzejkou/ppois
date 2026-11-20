#pragma once
#include <string>
class DeliveryNote;
class PurchaseOrder;
class GoodsReceipt {
public:
    GoodsReceipt(std::string receiptId, std::string purchaseOrderId, std::string inspectorId);
    bool acceptDelivery(DeliveryNote& note, PurchaseOrder& order, const std::string& receivedDate);
    bool rejectLine(const std::string& sku, int quantity, const std::string& reason);
    double calculateAcceptedValue(const PurchaseOrder& order) const noexcept;
    int acceptedQuantity() const noexcept;
    int rejectedQuantity() const noexcept;
    const std::string& receiptId() const noexcept;
    const std::string& purchaseOrderId() const noexcept;
    const std::string& inspectorId() const noexcept;
    const std::string& rejectionReason() const noexcept;
    bool completed() const noexcept;
private:
    std::string receiptId_;
    std::string purchaseOrderId_;
    std::string inspectorId_;
    std::string rejectionReason_;
    int acceptedQuantity_{0};
    int rejectedQuantity_{0};
    bool completed_{false};
};
