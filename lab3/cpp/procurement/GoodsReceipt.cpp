#include "GoodsReceipt.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

GoodsReceipt::GoodsReceipt(std::string receiptId, std::string purchaseOrderId, int quantity, double amount)
    : receiptId_(std::move(receiptId)), purchaseOrderId_(std::move(purchaseOrderId)),
      quantity_(quantity), amount_(amount) {
    if (receiptId_.empty() || purchaseOrderId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating GoodsReceipt");
}

bool GoodsReceipt::acceptDelivery() const {
    return active_ && !receiptId_.empty();
}

bool GoodsReceipt::calculateAcceptedValue(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& GoodsReceipt::receiptId() const noexcept { return receiptId_; }
const std::string& GoodsReceipt::purchaseOrderId() const noexcept { return purchaseOrderId_; }
int GoodsReceipt::quantity() const noexcept { return quantity_; }
double GoodsReceipt::amount() const noexcept { return amount_; }
bool GoodsReceipt::active() const noexcept { return active_; }

void GoodsReceipt::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool GoodsReceipt::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t GoodsReceipt::relatedCount() const noexcept { return relatedIds_.size(); }
