#include "Rack.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Rack::Rack(std::string rackCode, std::string zoneCode, int quantity, double amount)
    : rackCode_(std::move(rackCode)), zoneCode_(std::move(zoneCode)),
      quantity_(quantity), amount_(amount) {
    if (rackCode_.empty() || zoneCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Rack");
}

double Rack::calculateLoad() const {
    return amount_ * static_cast<double>(quantity_);
}

bool Rack::hasFreeLevel(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Rack::rackCode() const noexcept { return rackCode_; }
const std::string& Rack::zoneCode() const noexcept { return zoneCode_; }
int Rack::quantity() const noexcept { return quantity_; }
double Rack::amount() const noexcept { return amount_; }
bool Rack::active() const noexcept { return active_; }

void Rack::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Rack::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Rack::relatedCount() const noexcept { return relatedIds_.size(); }
