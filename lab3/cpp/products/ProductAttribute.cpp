#include "ProductAttribute.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

ProductAttribute::ProductAttribute(std::string attributeCode, std::string value, int quantity, double amount)
    : attributeCode_(std::move(attributeCode)), value_(std::move(value)),
      quantity_(quantity), amount_(amount) {
    if (attributeCode_.empty() || value_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating ProductAttribute");
}

bool ProductAttribute::validateValue() const {
    return active_ && !attributeCode_ .empty() && quantity_ >= 0;
}

bool ProductAttribute::normalizeValue(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& ProductAttribute::attributeCode() const noexcept { return attributeCode_; }
const std::string& ProductAttribute::value() const noexcept { return value_; }
int ProductAttribute::quantity() const noexcept { return quantity_; }
double ProductAttribute::amount() const noexcept { return amount_; }
bool ProductAttribute::active() const noexcept { return active_; }

void ProductAttribute::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool ProductAttribute::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t ProductAttribute::relatedCount() const noexcept { return relatedIds_.size(); }
