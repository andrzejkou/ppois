#pragma once
#include <string>
#include <vector>
class PurchaseRequest;
class Supplier {
public:
    Supplier(std::string supplierId, std::string companyName, std::string contactEmail,
             int leadTimeDays, double reliabilityRating);
    bool rateReliability(double newRating) noexcept;
    bool canFulfil(const PurchaseRequest& request) const noexcept;
    bool deactivate() noexcept;
    void recordOnTimeDelivery() noexcept;
    void recordLateDelivery() noexcept;
    double reliabilityScore() const noexcept;
    const std::string& supplierId() const noexcept;
    const std::string& companyName() const noexcept;
    const std::string& contactEmail() const noexcept;
    int leadTimeDays() const noexcept;
    bool active() const noexcept;
private:
    std::string supplierId_;
    std::string companyName_;
    std::string contactEmail_;
    int leadTimeDays_{0};
    double reliabilityRating_{0.0};
    std::size_t onTimeDeliveries_{0};
    std::size_t lateDeliveries_{0};
    bool active_{true};
};
