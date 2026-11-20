#include "GoodsReceipt.h"
#include "DeliveryNote.h"
#include "PurchaseOrder.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
GoodsReceipt::GoodsReceipt(std::string receiptId, std::string purchaseOrderId, std::string inspectorId)
    : receiptId_(std::move(receiptId)), purchaseOrderId_(std::move(purchaseOrderId)),
      inspectorId_(std::move(inspectorId)) {
    if (receiptId_.empty() || purchaseOrderId_.empty() || inspectorId_.empty())
        throw WarehouseException("Goods receipt identifiers are required");
}
bool GoodsReceipt::acceptDelivery(DeliveryNote& note, PurchaseOrder& order, const std::string& receivedDate) {
    if (completed_ || receivedDate.size() != 10 || order.id() != purchaseOrderId_ || !note.validateAgainstOrder(order)) return false;
    for (const auto& line : note.lines()) {
        order.registerReceipt(line.sku, line.deliveredQuantity);
        acceptedQuantity_ += line.deliveredQuantity;
    }
    if (!note.markReceived(receivedDate)) return false;
    completed_ = true; return true;
}
bool GoodsReceipt::rejectLine(const std::string& sku, int quantity, const std::string& reason) {
    if (completed_ || sku.empty() || quantity <= 0 || reason.empty()) return false;
    rejectedQuantity_ += quantity; rejectionReason_ = reason; return true;
}
double GoodsReceipt::calculateAcceptedValue(const PurchaseOrder& order) const noexcept {
    double total = 0.0;
    for (const auto& line : order.lines()) {
        const int accepted = std::min(line.received(), line.ordered());
        total += static_cast<double>(accepted) * (line.ordered() > 0 ? line.lineTotal() / static_cast<double>(line.ordered()) : 0.0);
    }
    return total;
}
int GoodsReceipt::acceptedQuantity() const noexcept { return acceptedQuantity_; }
int GoodsReceipt::rejectedQuantity() const noexcept { return rejectedQuantity_; }
const std::string& GoodsReceipt::receiptId() const noexcept { return receiptId_; }
const std::string& GoodsReceipt::purchaseOrderId() const noexcept { return purchaseOrderId_; }
const std::string& GoodsReceipt::inspectorId() const noexcept { return inspectorId_; }
const std::string& GoodsReceipt::rejectionReason() const noexcept { return rejectionReason_; }
bool GoodsReceipt::completed() const noexcept { return completed_; }
