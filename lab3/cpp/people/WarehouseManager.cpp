#include "WarehouseManager.h"
#include "procurement/PurchaseRequest.h"
#include "finance/WarehouseExpense.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
WarehouseManager::WarehouseManager(std::string employeeId, std::string departmentCode,
                                   double approvalLimit, std::size_t teamSize)
    : employeeId_(std::move(employeeId)), departmentCode_(std::move(departmentCode)),
      approvalLimit_(approvalLimit) {
    if (employeeId_.empty() || departmentCode_.empty() || approvalLimit_ < 0.0)
        throw WarehouseException("Invalid warehouse manager data");
    teamMemberIds_.reserve(teamSize);
}
bool WarehouseManager::approvePurchaseRequest(PurchaseRequest& request) {
    if (!canApprove(request.estimatedCost())) return false;
    return request.approve(employeeId_);
}
bool WarehouseManager::approveExpense(WarehouseExpense& expense) {
    return canApprove(expense.amount()) && expense.approveBy(employeeId_);
}
bool WarehouseManager::canApprove(double amount) const noexcept {
    return amount >= 0.0 && amount <= approvalLimit_;
}
void WarehouseManager::registerTeamMember(const std::string& employeeId) {
    if (!employeeId.empty() && employeeId != employeeId_ &&
        std::find(teamMemberIds_.begin(), teamMemberIds_.end(), employeeId) == teamMemberIds_.end())
        teamMemberIds_.push_back(employeeId);
}
double WarehouseManager::calculateTeamLoad(double totalAssignedTasks) const noexcept {
    return teamMemberIds_.empty() ? 0.0 : totalAssignedTasks / static_cast<double>(teamMemberIds_.size());
}
const std::string& WarehouseManager::employeeId() const noexcept { return employeeId_; }
const std::string& WarehouseManager::departmentCode() const noexcept { return departmentCode_; }
double WarehouseManager::approvalLimit() const noexcept { return approvalLimit_; }
