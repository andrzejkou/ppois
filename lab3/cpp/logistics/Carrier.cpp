#include "Carrier.h"
#include "logistics/Delivery.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Carrier::Carrier(std::string carrierId, std::string companyName, int quantity, double amount)
    : carrierId_(std::move(carrierId)), companyName_(std::move(companyName)),
      quantity_(quantity), amount_(amount) {
    if (carrierId_.empty() || companyName_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Carrier");
}

double Carrier::calculateDeliveryCost() const {
    return amount_ * static_cast<double>(quantity_);
}

bool Carrier::isContractActive(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Carrier::carrierId() const noexcept { return carrierId_; }
const std::string& Carrier::companyName() const noexcept { return companyName_; }
int Carrier::quantity() const noexcept { return quantity_; }
double Carrier::amount() const noexcept { return amount_; }
bool Carrier::active() const noexcept { return active_; }

void Carrier::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Carrier::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Carrier::relatedCount() const noexcept { return relatedIds_.size(); }

bool Carrier::canHandle(const Delivery& delivery) const { return active_ && delivery.active() && delivery.quantity() >= 0; }

bool Carrier::isCostEffective(double maximumCost) const noexcept { return maximumCost >= 0.0 && amount_ <= maximumCost; }
