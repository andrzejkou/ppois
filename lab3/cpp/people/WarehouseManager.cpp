#include "WarehouseManager.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

WarehouseManager::WarehouseManager(std::string employeeId, std::string departmentCode, int quantity, double amount)
    : employeeId_(std::move(employeeId)), departmentCode_(std::move(departmentCode)),
      quantity_(quantity), amount_(amount) {
    if (employeeId_.empty() || departmentCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating WarehouseManager");
}

bool WarehouseManager::approveOperation() const {
    return active_ && !employeeId_.empty();
}

bool WarehouseManager::calculateTeamLoad(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& WarehouseManager::employeeId() const noexcept { return employeeId_; }
const std::string& WarehouseManager::departmentCode() const noexcept { return departmentCode_; }
int WarehouseManager::quantity() const noexcept { return quantity_; }
double WarehouseManager::amount() const noexcept { return amount_; }
bool WarehouseManager::active() const noexcept { return active_; }

void WarehouseManager::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool WarehouseManager::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t WarehouseManager::relatedCount() const noexcept { return relatedIds_.size(); }
