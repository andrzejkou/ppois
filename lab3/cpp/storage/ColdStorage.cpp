#include "ColdStorage.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

ColdStorage::ColdStorage(std::string storageCode, std::string temperature, int quantity, double amount)
    : storageCode_(std::move(storageCode)), temperature_(std::move(temperature)),
      quantity_(quantity), amount_(amount) {
    if (storageCode_.empty() || temperature_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating ColdStorage");
}

bool ColdStorage::validateTemperature() const {
    return active_ && !storageCode_ .empty() && quantity_ >= 0;
}

bool ColdStorage::estimateEnergyUse(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& ColdStorage::storageCode() const noexcept { return storageCode_; }
const std::string& ColdStorage::temperature() const noexcept { return temperature_; }
int ColdStorage::quantity() const noexcept { return quantity_; }
double ColdStorage::amount() const noexcept { return amount_; }
bool ColdStorage::active() const noexcept { return active_; }

void ColdStorage::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool ColdStorage::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t ColdStorage::relatedCount() const noexcept { return relatedIds_.size(); }
