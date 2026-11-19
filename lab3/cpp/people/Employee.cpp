#include "Employee.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Employee::Employee(std::string employeeId, std::string fullName, int quantity, double amount)
    : employeeId_(std::move(employeeId)), fullName_(std::move(fullName)),
      quantity_(quantity), amount_(amount) {
    if (employeeId_.empty() || fullName_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Employee");
}

bool Employee::validateIdentity() const {
    return active_ && !employeeId_ .empty() && quantity_ >= 0;
}

bool Employee::changeRole(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Employee::employeeId() const noexcept { return employeeId_; }
const std::string& Employee::fullName() const noexcept { return fullName_; }
int Employee::quantity() const noexcept { return quantity_; }
double Employee::amount() const noexcept { return amount_; }
bool Employee::active() const noexcept { return active_; }

void Employee::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Employee::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Employee::relatedCount() const noexcept { return relatedIds_.size(); }
