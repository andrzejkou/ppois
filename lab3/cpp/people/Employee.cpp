#include "Employee.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>

Employee::Employee(std::string employeeId, std::string fullName, std::string position,
                   double hourlyRate)
    : employeeId_(std::move(employeeId)), fullName_(std::move(fullName)),
      position_(std::move(position)), hourlyRate_(hourlyRate) {
    if (employeeId_.empty() || fullName_.empty() || position_.empty() || hourlyRate_ < 0.0)
        throw WarehouseException("Invalid employee data");
}
bool Employee::validateIdentity() const noexcept {
    return !employeeId_.empty() && !fullName_.empty() && active_;
}
bool Employee::changePosition(const std::string& newPosition) {
    if (!active_ || newPosition.empty()) return false;
    position_ = newPosition; return true;
}
bool Employee::changeHourlyRate(double newRate) noexcept {
    if (!active_ || newRate < 0.0) return false;
    hourlyRate_ = newRate; return true;
}
void Employee::deactivate() noexcept { active_ = false; }
void Employee::addSkill(const std::string& skill) {
    if (!skill.empty() && !hasSkill(skill)) skills_.push_back(skill);
}
bool Employee::hasSkill(const std::string& skill) const {
    return std::find(skills_.begin(), skills_.end(), skill) != skills_.end();
}
const std::string& Employee::employeeId() const noexcept { return employeeId_; }
const std::string& Employee::fullName() const noexcept { return fullName_; }
const std::string& Employee::position() const noexcept { return position_; }
double Employee::hourlyRate() const noexcept { return hourlyRate_; }
bool Employee::active() const noexcept { return active_; }
const std::vector<std::string>& Employee::skills() const noexcept { return skills_; }
