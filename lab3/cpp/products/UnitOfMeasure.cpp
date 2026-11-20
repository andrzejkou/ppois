#include "UnitOfMeasure.h"
#include "products/Product.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

UnitOfMeasure::UnitOfMeasure(std::string unitCode, std::string symbol, int quantity, double amount)
    : unitCode_(std::move(unitCode)), symbol_(std::move(symbol)),
      quantity_(quantity), amount_(amount) {
    if (unitCode_.empty() || symbol_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating UnitOfMeasure");
}

bool UnitOfMeasure::convertToBase() const {
    return active_ && !unitCode_.empty();
}

bool UnitOfMeasure::isCompatible(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& UnitOfMeasure::unitCode() const noexcept { return unitCode_; }
const std::string& UnitOfMeasure::symbol() const noexcept { return symbol_; }
int UnitOfMeasure::quantity() const noexcept { return quantity_; }
double UnitOfMeasure::amount() const noexcept { return amount_; }
bool UnitOfMeasure::active() const noexcept { return active_; }

void UnitOfMeasure::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool UnitOfMeasure::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t UnitOfMeasure::relatedCount() const noexcept { return relatedIds_.size(); }

bool UnitOfMeasure::canMeasure(const Product& product) const { return active_ && product.active() && !unitCode_.empty() && !symbol_.empty() && amount_ > 0.0; }

double UnitOfMeasure::toBase(double value) const noexcept { return active_ && value >= 0.0 ? value * amount_ : 0.0; }
