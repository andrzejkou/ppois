#include "StorageBin.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StorageBin::StorageBin(std::string binCode, std::string shelfCode, int quantity, double amount)
    : binCode_(std::move(binCode)), shelfCode_(std::move(shelfCode)),
      quantity_(quantity), amount_(amount) {
    if (binCode_.empty() || shelfCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StorageBin");
}

double StorageBin::calculateFreeCapacity() const {
    return amount_ * static_cast<double>(quantity_);
}

bool StorageBin::canFit(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& StorageBin::binCode() const noexcept { return binCode_; }
const std::string& StorageBin::shelfCode() const noexcept { return shelfCode_; }
int StorageBin::quantity() const noexcept { return quantity_; }
double StorageBin::amount() const noexcept { return amount_; }
bool StorageBin::active() const noexcept { return active_; }

void StorageBin::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StorageBin::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StorageBin::relatedCount() const noexcept { return relatedIds_.size(); }
