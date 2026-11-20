#pragma once
#include <string>
class Supplier;
class SupplierContract {
public:
    SupplierContract(std::string contractId, std::string supplierId,
                     std::string validFrom, std::string validUntil,
                     double minimumOrderValue, double latePenaltyRate);
    bool isValidOn(const std::string& date) const noexcept;
    double calculatePenalty(int overdueDays, double orderValue) const noexcept;
    bool belongsTo(const Supplier& supplier) const noexcept;
    bool renew(const std::string& newValidUntil);
    const std::string& contractId() const noexcept;
    const std::string& supplierId() const noexcept;
    double minimumOrderValue() const noexcept;
private:
    std::string contractId_;
    std::string supplierId_;
    std::string validFrom_;
    std::string validUntil_;
    double minimumOrderValue_{0.0};
    double latePenaltyRate_{0.0};
};
