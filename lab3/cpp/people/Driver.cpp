#include "Driver.h"
#include "logistics/Delivery.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
Driver::Driver(std::string employeeId, std::string licenseNumber,
               std::string licenseExpiry, double loadLimitKg)
    : employeeId_(std::move(employeeId)), licenseNumber_(std::move(licenseNumber)),
      licenseExpiry_(std::move(licenseExpiry)), loadLimitKg_(loadLimitKg) {
    if (employeeId_.empty() || licenseNumber_.empty() || licenseExpiry_.size() != 10 || loadLimitKg_ <= 0)
        throw WarehouseException("Invalid driver or driving licence data");
}
bool Driver::validateLicense(const std::string& onDate) const noexcept {
    return onDate.size() == 10 && onDate <= licenseExpiry_;
}
bool Driver::canCarry(double cargoWeightKg) const noexcept {
    return cargoWeightKg >= 0.0 && cargoWeightKg <= loadLimitKg_;
}
bool Driver::assignDelivery(const Delivery& delivery) {
    const auto& id = delivery.deliveryId();
    if (id.empty() || std::find(assignedDeliveryIds_.begin(), assignedDeliveryIds_.end(), id) != assignedDeliveryIds_.end())
        return false;
    assignedDeliveryIds_.push_back(id); return true;
}
bool Driver::recordDelivery(const std::string& deliveryId) {
    auto it = std::find(assignedDeliveryIds_.begin(), assignedDeliveryIds_.end(), deliveryId);
    if (it == assignedDeliveryIds_.end()) return false;
    assignedDeliveryIds_.erase(it);
    completedDeliveryIds_.push_back(deliveryId); return true;
}
std::size_t Driver::completedDeliveries() const noexcept { return completedDeliveryIds_.size(); }
const std::string& Driver::employeeId() const noexcept { return employeeId_; }
const std::string& Driver::licenseNumber() const noexcept { return licenseNumber_; }
const std::string& Driver::licenseExpiry() const noexcept { return licenseExpiry_; }
double Driver::loadLimitKg() const noexcept { return loadLimitKg_; }
