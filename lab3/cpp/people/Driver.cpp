#include "Driver.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Driver::Driver(std::string employeeId, std::string licenseNumber, int quantity, double amount)
    : employeeId_(std::move(employeeId)), licenseNumber_(std::move(licenseNumber)),
      quantity_(quantity), amount_(amount) {
    if (employeeId_.empty() || licenseNumber_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Driver");
}

bool Driver::validateLicense() const {
    return active_ && !employeeId_ .empty() && quantity_ >= 0;
}

bool Driver::recordDelivery(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Driver::employeeId() const noexcept { return employeeId_; }
const std::string& Driver::licenseNumber() const noexcept { return licenseNumber_; }
int Driver::quantity() const noexcept { return quantity_; }
double Driver::amount() const noexcept { return amount_; }
bool Driver::active() const noexcept { return active_; }

void Driver::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Driver::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Driver::relatedCount() const noexcept { return relatedIds_.size(); }
