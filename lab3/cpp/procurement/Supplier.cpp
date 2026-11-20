#include "Supplier.h"
#include "PurchaseRequest.h"
#include "exceptions/WarehouseException.h"
#include <utility>
Supplier::Supplier(std::string supplierId, std::string companyName, std::string contactEmail,
                   int leadTimeDays, double reliabilityRating)
    : supplierId_(std::move(supplierId)), companyName_(std::move(companyName)),
      contactEmail_(std::move(contactEmail)), leadTimeDays_(leadTimeDays),
      reliabilityRating_(reliabilityRating) {
    if (supplierId_.empty() || companyName_.empty() || leadTimeDays_ < 0 ||
        reliabilityRating_ < 0.0 || reliabilityRating_ > 5.0)
        throw WarehouseException("Invalid supplier data");
}
bool Supplier::rateReliability(double rating) noexcept {
    if (rating < 0.0 || rating > 5.0) return false;
    reliabilityRating_ = rating; return true;
}
bool Supplier::canFulfil(const PurchaseRequest& request) const noexcept {
    return active_ && request.status() == PurchaseRequestStatus::Submitted &&
           request.estimatedCost() >= 0.0;
}
bool Supplier::deactivate() noexcept { if (!active_) return false; active_ = false; return true; }
void Supplier::recordOnTimeDelivery() noexcept { ++onTimeDeliveries_; }
void Supplier::recordLateDelivery() noexcept { ++lateDeliveries_; }
double Supplier::reliabilityScore() const noexcept {
    const auto total = onTimeDeliveries_ + lateDeliveries_;
    return total == 0 ? reliabilityRating_ :
        5.0 * static_cast<double>(onTimeDeliveries_) / static_cast<double>(total);
}
const std::string& Supplier::supplierId() const noexcept { return supplierId_; }
const std::string& Supplier::companyName() const noexcept { return companyName_; }
const std::string& Supplier::contactEmail() const noexcept { return contactEmail_; }
int Supplier::leadTimeDays() const noexcept { return leadTimeDays_; }
bool Supplier::active() const noexcept { return active_; }
