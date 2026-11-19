#include "Customer.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Customer::Customer(std::string customerId, std::string fullName, int quantity, double amount)
    : customerId_(std::move(customerId)), fullName_(std::move(fullName)),
      quantity_(quantity), amount_(amount) {
    if (customerId_.empty() || fullName_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Customer");
}

bool Customer::validateContact() const {
    return active_ && !customerId_ .empty() && quantity_ >= 0;
}

bool Customer::updateLoyalty(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Customer::customerId() const noexcept { return customerId_; }
const std::string& Customer::fullName() const noexcept { return fullName_; }
int Customer::quantity() const noexcept { return quantity_; }
double Customer::amount() const noexcept { return amount_; }
bool Customer::active() const noexcept { return active_; }

void Customer::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Customer::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Customer::relatedCount() const noexcept { return relatedIds_.size(); }
