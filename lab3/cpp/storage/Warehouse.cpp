#include "Warehouse.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Warehouse::Warehouse(std::string warehouseCode, std::string address, int quantity, double amount)
    : warehouseCode_(std::move(warehouseCode)), address_(std::move(address)),
      quantity_(quantity), amount_(amount) {
    if (warehouseCode_.empty() || address_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Warehouse");
}

double Warehouse::calculateFreeCapacity() const {
    return amount_ * static_cast<double>(quantity_);
}

bool Warehouse::canAccept(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Warehouse::warehouseCode() const noexcept { return warehouseCode_; }
const std::string& Warehouse::address() const noexcept { return address_; }
int Warehouse::quantity() const noexcept { return quantity_; }
double Warehouse::amount() const noexcept { return amount_; }
bool Warehouse::active() const noexcept { return active_; }

void Warehouse::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Warehouse::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Warehouse::relatedCount() const noexcept { return relatedIds_.size(); }
