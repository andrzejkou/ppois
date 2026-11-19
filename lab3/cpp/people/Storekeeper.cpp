#include "Storekeeper.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Storekeeper::Storekeeper(std::string employeeId, std::string zoneCode, int quantity, double amount)
    : employeeId_(std::move(employeeId)), zoneCode_(std::move(zoneCode)),
      quantity_(quantity), amount_(amount) {
    if (employeeId_.empty() || zoneCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Storekeeper");
}

bool Storekeeper::assignZone() const {
    return active_ && !employeeId_.empty();
}

bool Storekeeper::countAssignedTasks(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Storekeeper::employeeId() const noexcept { return employeeId_; }
const std::string& Storekeeper::zoneCode() const noexcept { return zoneCode_; }
int Storekeeper::quantity() const noexcept { return quantity_; }
double Storekeeper::amount() const noexcept { return amount_; }
bool Storekeeper::active() const noexcept { return active_; }

void Storekeeper::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Storekeeper::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Storekeeper::relatedCount() const noexcept { return relatedIds_.size(); }
