#include "Supplier.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Supplier::Supplier(std::string supplierId, std::string companyName, int quantity, double amount)
    : supplierId_(std::move(supplierId)), companyName_(std::move(companyName)),
      quantity_(quantity), amount_(amount) {
    if (supplierId_.empty() || companyName_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Supplier");
}

double Supplier::rateReliability() const {
    return amount_ * static_cast<double>(quantity_);
}

bool Supplier::canSupply(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Supplier::supplierId() const noexcept { return supplierId_; }
const std::string& Supplier::companyName() const noexcept { return companyName_; }
int Supplier::quantity() const noexcept { return quantity_; }
double Supplier::amount() const noexcept { return amount_; }
bool Supplier::active() const noexcept { return active_; }

void Supplier::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Supplier::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Supplier::relatedCount() const noexcept { return relatedIds_.size(); }
