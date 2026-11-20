#include "ProductCategory.h"
#include "products/Product.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

ProductCategory::ProductCategory(std::string categoryCode, std::string name, int quantity, double amount)
    : categoryCode_(std::move(categoryCode)), name_(std::move(name)),
      quantity_(quantity), amount_(amount) {
    if (categoryCode_.empty() || name_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating ProductCategory");
}

bool ProductCategory::addProduct() const {
    return active_ && !categoryCode_.empty();
}

bool ProductCategory::removeProduct(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& ProductCategory::categoryCode() const noexcept { return categoryCode_; }
const std::string& ProductCategory::name() const noexcept { return name_; }
int ProductCategory::quantity() const noexcept { return quantity_; }
double ProductCategory::amount() const noexcept { return amount_; }
bool ProductCategory::active() const noexcept { return active_; }

void ProductCategory::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool ProductCategory::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t ProductCategory::relatedCount() const noexcept { return relatedIds_.size(); }

bool ProductCategory::containsProduct(const Product& product) const { return active_ && product.active() && product.categoryCode() == categoryCode_; }

bool ProductCategory::isPopulated() const noexcept { return active_ && quantity_ > 0; }
