#include "PurchaseOrderLine.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

PurchaseOrderLine::PurchaseOrderLine(std::string lineId, std::string productSku, int quantity, double amount)
    : lineId_(std::move(lineId)), productSku_(std::move(productSku)),
      quantity_(quantity), amount_(amount) {
    if (lineId_.empty() || productSku_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating PurchaseOrderLine");
}

double PurchaseOrderLine::calculateLineTotal() const {
    return amount_ * static_cast<double>(quantity_);
}

bool PurchaseOrderLine::changeQuantity(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& PurchaseOrderLine::lineId() const noexcept { return lineId_; }
const std::string& PurchaseOrderLine::productSku() const noexcept { return productSku_; }
int PurchaseOrderLine::quantity() const noexcept { return quantity_; }
double PurchaseOrderLine::amount() const noexcept { return amount_; }
bool PurchaseOrderLine::active() const noexcept { return active_; }

void PurchaseOrderLine::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool PurchaseOrderLine::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t PurchaseOrderLine::relatedCount() const noexcept { return relatedIds_.size(); }
