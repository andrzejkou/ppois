#include "SalesOrder.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

SalesOrder::SalesOrder(std::string orderId, std::string customerId, int quantity, double amount)
    : orderId_(std::move(orderId)), customerId_(std::move(customerId)),
      quantity_(quantity), amount_(amount) {
    if (orderId_.empty() || customerId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating SalesOrder");
}

double SalesOrder::calculateTotal() const {
    return amount_ * static_cast<double>(quantity_);
}

bool SalesOrder::cancelOrder(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& SalesOrder::orderId() const noexcept { return orderId_; }
const std::string& SalesOrder::customerId() const noexcept { return customerId_; }
int SalesOrder::quantity() const noexcept { return quantity_; }
double SalesOrder::amount() const noexcept { return amount_; }
bool SalesOrder::active() const noexcept { return active_; }

void SalesOrder::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool SalesOrder::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t SalesOrder::relatedCount() const noexcept { return relatedIds_.size(); }
