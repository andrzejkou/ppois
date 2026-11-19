#include "WarehouseWorker.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

WarehouseWorker::WarehouseWorker(std::string employeeId, std::string shiftCode, int quantity, double amount)
    : employeeId_(std::move(employeeId)), shiftCode_(std::move(shiftCode)),
      quantity_(quantity), amount_(amount) {
    if (employeeId_.empty() || shiftCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating WarehouseWorker");
}

bool WarehouseWorker::startShift() const {
    return active_ && !employeeId_.empty();
}

bool WarehouseWorker::finishShift(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& WarehouseWorker::employeeId() const noexcept { return employeeId_; }
const std::string& WarehouseWorker::shiftCode() const noexcept { return shiftCode_; }
int WarehouseWorker::quantity() const noexcept { return quantity_; }
double WarehouseWorker::amount() const noexcept { return amount_; }
bool WarehouseWorker::active() const noexcept { return active_; }

void WarehouseWorker::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool WarehouseWorker::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t WarehouseWorker::relatedCount() const noexcept { return relatedIds_.size(); }
