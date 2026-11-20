#pragma once
#include <string>
#include <vector>
enum class PurchaseRequestStatus { Draft, Submitted, Approved, Rejected, Cancelled };
struct PurchaseRequestLine {
    std::string sku;
    int quantity{0};
    double estimatedUnitPrice{0.0};
};
class PurchaseRequest {
public:
    PurchaseRequest(std::string requestId, std::string departmentCode, std::string requesterId);
    bool addLine(const PurchaseRequestLine& line);
    bool submit();
    bool approve(const std::string& managerId);
    bool reject(const std::string& managerId, const std::string& reason);
    bool cancel();
    double estimatedCost() const noexcept;
    bool hasLine(const std::string& sku) const noexcept;
    PurchaseRequestStatus status() const noexcept;
    const std::string& requestId() const noexcept;
    const std::string& departmentCode() const noexcept;
    const std::string& requesterId() const noexcept;
    const std::string& approverId() const noexcept;
    const std::string& rejectionReason() const noexcept;
    const std::vector<PurchaseRequestLine>& lines() const noexcept;
private:
    std::string requestId_;
    std::string departmentCode_;
    std::string requesterId_;
    std::string approverId_;
    std::string rejectionReason_;
    std::vector<PurchaseRequestLine> lines_;
    PurchaseRequestStatus status_{PurchaseRequestStatus::Draft};
};
