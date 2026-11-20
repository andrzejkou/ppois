#include "Shelf.h"
#include "storage/Rack.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Shelf::Shelf(std::string shelfCode, std::string rackCode, int quantity, double amount)
    : shelfCode_(std::move(shelfCode)), rackCode_(std::move(rackCode)),
      quantity_(quantity), amount_(amount) {
    if (shelfCode_.empty() || rackCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Shelf");
}

double Shelf::calculateFreeVolume() const {
    return amount_ * static_cast<double>(quantity_);
}

bool Shelf::canStore(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Shelf::shelfCode() const noexcept { return shelfCode_; }
const std::string& Shelf::rackCode() const noexcept { return rackCode_; }
int Shelf::quantity() const noexcept { return quantity_; }
double Shelf::amount() const noexcept { return amount_; }
bool Shelf::active() const noexcept { return active_; }

void Shelf::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Shelf::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Shelf::relatedCount() const noexcept { return relatedIds_.size(); }

bool Shelf::belongsTo(const Rack& rack) const { return active_ && rack.active() && rack.rackCode() == rackCode_; }

bool Shelf::hasCapacityFor(double volume) const noexcept { return active_ && volume >= 0.0 && volume <= amount_; }
