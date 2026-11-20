#include "AccessRole.h"
#include "people/Employee.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

AccessRole::AccessRole(std::string roleCode, std::string roleName, int quantity, double amount)
    : roleCode_(std::move(roleCode)), roleName_(std::move(roleName)),
      quantity_(quantity), amount_(amount) {
    if (roleCode_.empty() || roleName_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating AccessRole");
}

bool AccessRole::grantPermission() const {
    return active_ && !roleCode_.empty();
}

bool AccessRole::revokePermission(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& AccessRole::roleCode() const noexcept { return roleCode_; }
const std::string& AccessRole::roleName() const noexcept { return roleName_; }
int AccessRole::quantity() const noexcept { return quantity_; }
double AccessRole::amount() const noexcept { return amount_; }
bool AccessRole::active() const noexcept { return active_; }

void AccessRole::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool AccessRole::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t AccessRole::relatedCount() const noexcept { return relatedIds_.size(); }

bool AccessRole::assignTo(const Employee& employee) { if (!active_ || !employee.active() || employee.employeeId().empty()) return false; addRelatedId(employee.employeeId()); return true; }

bool AccessRole::hasPermissionCount(int required) const noexcept { return active_ && required >= 0 && quantity_ >= required; }
