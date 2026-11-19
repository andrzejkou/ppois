#include "Product.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Product::Product(std::string sku, std::string name, int quantity, double amount)
    : sku_(std::move(sku)), name_(std::move(name)),
      quantity_(quantity), amount_(amount) {
    if (sku_.empty() || name_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Product");
}

bool Product::changePrice() const {
    return active_ && !sku_.empty();
}

bool Product::isAvailableForSale(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Product::sku() const noexcept { return sku_; }
const std::string& Product::name() const noexcept { return name_; }
int Product::quantity() const noexcept { return quantity_; }
double Product::amount() const noexcept { return amount_; }
bool Product::active() const noexcept { return active_; }

void Product::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Product::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Product::relatedCount() const noexcept { return relatedIds_.size(); }
