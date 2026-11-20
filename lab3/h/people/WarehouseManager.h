#pragma once
#include <string>
#include <vector>
class PurchaseRequest;
class WarehouseExpense;
class WarehouseManager {
public:
    WarehouseManager(std::string employeeId, std::string departmentCode,
                     double approvalLimit, std::size_t teamSize);
    bool approvePurchaseRequest(PurchaseRequest& request);
    bool approveExpense(WarehouseExpense& expense);
    bool canApprove(double amount) const noexcept;
    void registerTeamMember(const std::string& employeeId);
    double calculateTeamLoad(double totalAssignedTasks) const noexcept;
    const std::string& employeeId() const noexcept;
    const std::string& departmentCode() const noexcept;
    double approvalLimit() const noexcept;
private:
    std::string employeeId_;
    std::string departmentCode_;
    double approvalLimit_{0.0};
    std::vector<std::string> teamMemberIds_;
};
