#include "DeliveryNote.h"
#include "PurchaseOrder.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
DeliveryNote::DeliveryNote(std::string noteNumber, std::string supplierId, std::string purchaseOrderId)
    : noteNumber_(std::move(noteNumber)), supplierId_(std::move(supplierId)),
      purchaseOrderId_(std::move(purchaseOrderId)) {
    if (noteNumber_.empty() || supplierId_.empty() || purchaseOrderId_.empty())
        throw WarehouseException("Delivery note identifiers are required");
}
bool DeliveryNote::addLine(const DeliveryNoteLine& line) {
    if (received_ || line.sku.empty() || line.deliveredQuantity <= 0) return false;
    auto it = std::find_if(lines_.begin(), lines_.end(), [&](const auto& current) { return current.sku == line.sku; });
    if (it == lines_.end()) lines_.push_back(line); else it->deliveredQuantity += line.deliveredQuantity;
    return true;
}
bool DeliveryNote::validateAgainstOrder(const PurchaseOrder& order) const noexcept {
    if (order.id() != purchaseOrderId_ || order.supplierId() != supplierId_ || lines_.empty()) return false;
    for (const auto& delivered : lines_) {
        auto it = std::find_if(order.lines().begin(), order.lines().end(),
            [&](const auto& ordered) { return ordered.sku() == delivered.sku; });
        if (it == order.lines().end() || delivered.deliveredQuantity > it->outstanding()) return false;
    }
    return true;
}
bool DeliveryNote::markReceived(const std::string& receivedDate) {
    if (received_ || receivedDate.size() != 10 || lines_.empty()) return false;
    receivedDate_ = receivedDate; received_ = true; return true;
}
int DeliveryNote::totalDeliveredQuantity() const noexcept {
    int total = 0; for (const auto& line : lines_) total += line.deliveredQuantity; return total;
}
const std::string& DeliveryNote::noteNumber() const noexcept { return noteNumber_; }
const std::string& DeliveryNote::supplierId() const noexcept { return supplierId_; }
const std::string& DeliveryNote::purchaseOrderId() const noexcept { return purchaseOrderId_; }
const std::string& DeliveryNote::receivedDate() const noexcept { return receivedDate_; }
const std::vector<DeliveryNoteLine>& DeliveryNote::lines() const noexcept { return lines_; }
bool DeliveryNote::received() const noexcept { return received_; }
