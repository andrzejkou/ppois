#include "SerialNumber.h"
#include "products/Product.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

SerialNumber::SerialNumber(std::string serialCode, std::string productSku, int quantity, double amount)
    : serialCode_(std::move(serialCode)), productSku_(std::move(productSku)),
      quantity_(quantity), amount_(amount) {
    if (serialCode_.empty() || productSku_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating SerialNumber");
}

bool SerialNumber::assignProduct() const {
    return active_ && !serialCode_.empty();
}

bool SerialNumber::isAssigned(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& SerialNumber::serialCode() const noexcept { return serialCode_; }
const std::string& SerialNumber::productSku() const noexcept { return productSku_; }
int SerialNumber::quantity() const noexcept { return quantity_; }
double SerialNumber::amount() const noexcept { return amount_; }
bool SerialNumber::active() const noexcept { return active_; }

void SerialNumber::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool SerialNumber::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t SerialNumber::relatedCount() const noexcept { return relatedIds_.size(); }

bool SerialNumber::belongsTo(const Product& product) const { return active_ && product.active() && product.sku() == productSku_; }

bool SerialNumber::isRegistered() const noexcept { return active_ && !serialCode_.empty() && !productSku_.empty(); }
