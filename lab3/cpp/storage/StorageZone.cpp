#include "StorageZone.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StorageZone::StorageZone(std::string zoneCode, std::string temperatureMode, int quantity, double amount)
    : zoneCode_(std::move(zoneCode)), temperatureMode_(std::move(temperatureMode)),
      quantity_(quantity), amount_(amount) {
    if (zoneCode_.empty() || temperatureMode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StorageZone");
}

bool StorageZone::isSuitableFor() const {
    return active_ && !zoneCode_ .empty() && quantity_ >= 0;
}

bool StorageZone::calculateOccupancy(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& StorageZone::zoneCode() const noexcept { return zoneCode_; }
const std::string& StorageZone::temperatureMode() const noexcept { return temperatureMode_; }
int StorageZone::quantity() const noexcept { return quantity_; }
double StorageZone::amount() const noexcept { return amount_; }
bool StorageZone::active() const noexcept { return active_; }

void StorageZone::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StorageZone::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StorageZone::relatedCount() const noexcept { return relatedIds_.size(); }
