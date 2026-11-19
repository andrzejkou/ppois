#include "StorageCondition.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StorageCondition::StorageCondition(std::string conditionCode, std::string description, int quantity, double amount)
    : conditionCode_(std::move(conditionCode)), description_(std::move(description)),
      quantity_(quantity), amount_(amount) {
    if (conditionCode_.empty() || description_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StorageCondition");
}

bool StorageCondition::isSatisfiedBy() const {
    return active_ && !conditionCode_ .empty() && quantity_ >= 0;
}

bool StorageCondition::requiresCooling(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& StorageCondition::conditionCode() const noexcept { return conditionCode_; }
const std::string& StorageCondition::description() const noexcept { return description_; }
int StorageCondition::quantity() const noexcept { return quantity_; }
double StorageCondition::amount() const noexcept { return amount_; }
bool StorageCondition::active() const noexcept { return active_; }

void StorageCondition::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StorageCondition::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StorageCondition::relatedCount() const noexcept { return relatedIds_.size(); }
