#include "SupplierContract.h"
#include "Supplier.h"
#include "exceptions/WarehouseException.h"
#include <utility>
SupplierContract::SupplierContract(std::string contractId, std::string supplierId,
                                   std::string validFrom, std::string validUntil,
                                   double minimumOrderValue, double latePenaltyRate)
    : contractId_(std::move(contractId)), supplierId_(std::move(supplierId)),
      validFrom_(std::move(validFrom)), validUntil_(std::move(validUntil)),
      minimumOrderValue_(minimumOrderValue), latePenaltyRate_(latePenaltyRate) {
    if (contractId_.empty() || supplierId_.empty() || validFrom_.size() != 10 ||
        validUntil_.size() != 10 || validUntil_ < validFrom_ ||
        minimumOrderValue_ < 0.0 || latePenaltyRate_ < 0.0 || latePenaltyRate_ > 1.0)
        throw WarehouseException("Invalid supplier contract");
}
bool SupplierContract::isValidOn(const std::string& date) const noexcept {
    return date.size() == 10 && date >= validFrom_ && date <= validUntil_;
}
double SupplierContract::calculatePenalty(int overdueDays, double orderValue) const noexcept {
    if (overdueDays <= 0 || orderValue < minimumOrderValue_) return 0.0;
    return orderValue * latePenaltyRate_ * static_cast<double>(overdueDays);
}
bool SupplierContract::belongsTo(const Supplier& supplier) const noexcept {
    return supplier.supplierId() == supplierId_;
}
bool SupplierContract::renew(const std::string& newValidUntil) {
    if (newValidUntil.size() != 10 || newValidUntil <= validUntil_) return false;
    validUntil_ = newValidUntil; return true;
}
const std::string& SupplierContract::contractId() const noexcept { return contractId_; }
const std::string& SupplierContract::supplierId() const noexcept { return supplierId_; }
double SupplierContract::minimumOrderValue() const noexcept { return minimumOrderValue_; }

const std::string& SupplierContract::validFrom() const noexcept { return validFrom_; }
const std::string& SupplierContract::validUntil() const noexcept { return validUntil_; }
double SupplierContract::latePenaltyRate() const noexcept { return latePenaltyRate_; }
