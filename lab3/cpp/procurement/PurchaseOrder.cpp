#include "PurchaseOrder.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

PurchaseOrder::PurchaseOrder(std::string purchaseOrderId, std::string supplierId, int quantity, double amount)
    : purchaseOrderId_(std::move(purchaseOrderId)), supplierId_(std::move(supplierId)),
      quantity_(quantity), amount_(amount) {
    if (purchaseOrderId_.empty() || supplierId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating PurchaseOrder");
}

double PurchaseOrder::calculateTotal() const {
    return amount_ * static_cast<double>(quantity_);
}

bool PurchaseOrder::closeOrder(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& PurchaseOrder::purchaseOrderId() const noexcept { return purchaseOrderId_; }
const std::string& PurchaseOrder::supplierId() const noexcept { return supplierId_; }
int PurchaseOrder::quantity() const noexcept { return quantity_; }
double PurchaseOrder::amount() const noexcept { return amount_; }
bool PurchaseOrder::active() const noexcept { return active_; }

void PurchaseOrder::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool PurchaseOrder::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t PurchaseOrder::relatedCount() const noexcept { return relatedIds_.size(); }
