#include "WarehouseSystem.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

WarehouseSystem::WarehouseSystem(std::string systemId, std::string systemName, int quantity, double amount)
    : systemId_(std::move(systemId)), systemName_(std::move(systemName)),
      quantity_(quantity), amount_(amount) {
    if (systemId_.empty() || systemName_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating WarehouseSystem");
}

bool WarehouseSystem::registerWarehouse() const {
    return active_ && !systemId_.empty();
}

bool WarehouseSystem::calculateNetworkStock(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& WarehouseSystem::systemId() const noexcept { return systemId_; }
const std::string& WarehouseSystem::systemName() const noexcept { return systemName_; }
int WarehouseSystem::quantity() const noexcept { return quantity_; }
double WarehouseSystem::amount() const noexcept { return amount_; }
bool WarehouseSystem::active() const noexcept { return active_; }

void WarehouseSystem::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool WarehouseSystem::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t WarehouseSystem::relatedCount() const noexcept { return relatedIds_.size(); }
