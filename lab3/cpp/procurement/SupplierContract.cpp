#include "SupplierContract.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

SupplierContract::SupplierContract(std::string contractId, std::string supplierId, int quantity, double amount)
    : contractId_(std::move(contractId)), supplierId_(std::move(supplierId)),
      quantity_(quantity), amount_(amount) {
    if (contractId_.empty() || supplierId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating SupplierContract");
}

bool SupplierContract::isValidOn() const {
    return active_ && !contractId_ .empty() && quantity_ >= 0;
}

bool SupplierContract::calculatePenalty(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& SupplierContract::contractId() const noexcept { return contractId_; }
const std::string& SupplierContract::supplierId() const noexcept { return supplierId_; }
int SupplierContract::quantity() const noexcept { return quantity_; }
double SupplierContract::amount() const noexcept { return amount_; }
bool SupplierContract::active() const noexcept { return active_; }

void SupplierContract::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool SupplierContract::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t SupplierContract::relatedCount() const noexcept { return relatedIds_.size(); }
