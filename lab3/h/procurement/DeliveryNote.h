#pragma once
#include <string>
#include <vector>
class PurchaseOrder;
struct DeliveryNoteLine {
    std::string sku;
    int deliveredQuantity{0};
};
class DeliveryNote {
public:
    DeliveryNote(std::string noteNumber, std::string supplierId, std::string purchaseOrderId);
    bool addLine(const DeliveryNoteLine& line);
    bool validateAgainstOrder(const PurchaseOrder& order) const noexcept;
    bool markReceived(const std::string& receivedDate);
    int totalDeliveredQuantity() const noexcept;
    const std::string& noteNumber() const noexcept;
    const std::string& supplierId() const noexcept;
    const std::string& purchaseOrderId() const noexcept;
    const std::string& receivedDate() const noexcept;
    const std::vector<DeliveryNoteLine>& lines() const noexcept;
    bool received() const noexcept;
private:
    std::string noteNumber_;
    std::string supplierId_;
    std::string purchaseOrderId_;
    std::string receivedDate_;
    std::vector<DeliveryNoteLine> lines_;
    bool received_{false};
};
