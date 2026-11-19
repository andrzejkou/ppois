#include "InventoryLine.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

InventoryLine::InventoryLine(std::string lineId, std::string productSku, int quantity, double amount)
    : lineId_(std::move(lineId)), productSku_(std::move(productSku)),
      quantity_(quantity), amount_(amount) {
    if (lineId_.empty() || productSku_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating InventoryLine");
}

bool InventoryLine::recordCount() const {
    return active_ && !lineId_.empty();
}

bool InventoryLine::calculateVariance(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& InventoryLine::lineId() const noexcept { return lineId_; }
const std::string& InventoryLine::productSku() const noexcept { return productSku_; }
int InventoryLine::quantity() const noexcept { return quantity_; }
double InventoryLine::amount() const noexcept { return amount_; }
bool InventoryLine::active() const noexcept { return active_; }

void InventoryLine::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool InventoryLine::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t InventoryLine::relatedCount() const noexcept { return relatedIds_.size(); }
