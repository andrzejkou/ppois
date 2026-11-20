#include "PurchaseRequest.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
PurchaseRequest::PurchaseRequest(std::string requestId, std::string departmentCode, std::string requesterId)
    : requestId_(std::move(requestId)), departmentCode_(std::move(departmentCode)),
      requesterId_(std::move(requesterId)) {
    if (requestId_.empty() || departmentCode_.empty() || requesterId_.empty())
        throw WarehouseException("Invalid purchase request data");
}
bool PurchaseRequest::addLine(const PurchaseRequestLine& line) {
    if (status_ != PurchaseRequestStatus::Draft || line.sku.empty() ||
        line.quantity <= 0 || line.estimatedUnitPrice < 0.0) return false;
    auto it = std::find_if(lines_.begin(), lines_.end(), [&](const auto& current) { return current.sku == line.sku; });
    if (it != lines_.end()) { it->quantity += line.quantity; it->estimatedUnitPrice = line.estimatedUnitPrice; }
    else lines_.push_back(line);
    return true;
}
bool PurchaseRequest::submit() {
    if (status_ != PurchaseRequestStatus::Draft || lines_.empty()) return false;
    status_ = PurchaseRequestStatus::Submitted; return true;
}
bool PurchaseRequest::approve(const std::string& managerId) {
    if (status_ != PurchaseRequestStatus::Submitted || managerId.empty()) return false;
    status_ = PurchaseRequestStatus::Approved; approverId_ = managerId; return true;
}
bool PurchaseRequest::reject(const std::string& managerId, const std::string& reason) {
    if (status_ != PurchaseRequestStatus::Submitted || managerId.empty() || reason.empty()) return false;
    status_ = PurchaseRequestStatus::Rejected; approverId_ = managerId; rejectionReason_ = reason; return true;
}
bool PurchaseRequest::cancel() {
    if (status_ == PurchaseRequestStatus::Approved || status_ == PurchaseRequestStatus::Cancelled) return false;
    status_ = PurchaseRequestStatus::Cancelled; return true;
}
double PurchaseRequest::estimatedCost() const noexcept {
    double total = 0.0; for (const auto& line : lines_) total += line.quantity * line.estimatedUnitPrice;
    return total;
}
bool PurchaseRequest::hasLine(const std::string& sku) const noexcept {
    return std::any_of(lines_.begin(), lines_.end(), [&](const auto& line) { return line.sku == sku; });
}
PurchaseRequestStatus PurchaseRequest::status() const noexcept { return status_; }
const std::string& PurchaseRequest::requestId() const noexcept { return requestId_; }
const std::string& PurchaseRequest::departmentCode() const noexcept { return departmentCode_; }
const std::string& PurchaseRequest::requesterId() const noexcept { return requesterId_; }
const std::string& PurchaseRequest::approverId() const noexcept { return approverId_; }
const std::string& PurchaseRequest::rejectionReason() const noexcept { return rejectionReason_; }
const std::vector<PurchaseRequestLine>& PurchaseRequest::lines() const noexcept { return lines_; }
