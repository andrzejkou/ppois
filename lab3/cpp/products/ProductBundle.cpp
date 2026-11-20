#include "ProductBundle.h"
#include "products/Product.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

ProductBundle::ProductBundle(std::string bundleCode, std::string name, int quantity, double amount)
    : bundleCode_(std::move(bundleCode)), name_(std::move(name)),
      quantity_(quantity), amount_(amount) {
    if (bundleCode_.empty() || name_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating ProductBundle");
}

double ProductBundle::calculateBundlePrice() const {
    return amount_;
}

bool ProductBundle::validateComponents(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& ProductBundle::bundleCode() const noexcept { return bundleCode_; }
const std::string& ProductBundle::name() const noexcept { return name_; }
int ProductBundle::quantity() const noexcept { return quantity_; }
double ProductBundle::amount() const noexcept { return amount_; }
bool ProductBundle::active() const noexcept { return active_; }

void ProductBundle::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool ProductBundle::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t ProductBundle::relatedCount() const noexcept { return relatedIds_.size(); }

bool ProductBundle::addComponent(const Product& product, int units) { if (!active_ || !product.active() || units <= 0) return false; addRelatedId(product.sku()); quantity_ += units; amount_ += product.unitPrice() * units; return true; }

bool ProductBundle::isAvailable(int requested) const noexcept { return active_ && requested > 0 && quantity_ >= requested; }
