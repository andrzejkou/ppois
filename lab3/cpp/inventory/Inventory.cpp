#include "Inventory.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Inventory::Inventory(std::string inventoryId, std::string warehouseCode, int quantity, double amount)
    : inventoryId_(std::move(inventoryId)), warehouseCode_(std::move(warehouseCode)),
      quantity_(quantity), amount_(amount) {
    if (inventoryId_.empty() || warehouseCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Inventory");
}

bool Inventory::startCounting() const {
    return active_ && !inventoryId_.empty();
}

bool Inventory::finishCounting(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& Inventory::inventoryId() const noexcept { return inventoryId_; }
const std::string& Inventory::warehouseCode() const noexcept { return warehouseCode_; }
int Inventory::quantity() const noexcept { return quantity_; }
double Inventory::amount() const noexcept { return amount_; }
bool Inventory::active() const noexcept { return active_; }

void Inventory::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Inventory::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Inventory::relatedCount() const noexcept { return relatedIds_.size(); }
