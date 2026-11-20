#pragma once
#include <string>
#include <vector>
class Delivery;
class Driver {
public:
    Driver(std::string employeeId, std::string licenseNumber,
           std::string licenseExpiry, double loadLimitKg);
    bool validateLicense(const std::string& onDate) const noexcept;
    bool canCarry(double cargoWeightKg) const noexcept;
    bool assignDelivery(const Delivery& delivery);
    bool recordDelivery(const std::string& deliveryId);
    std::size_t completedDeliveries() const noexcept;
    const std::string& employeeId() const noexcept;
    const std::string& licenseNumber() const noexcept;
    const std::string& licenseExpiry() const noexcept;
    double loadLimitKg() const noexcept;
private:
    std::string employeeId_;
    std::string licenseNumber_;
    std::string licenseExpiry_; // ISO YYYY-MM-DD
    double loadLimitKg_{0.0};
    std::vector<std::string> assignedDeliveryIds_;
    std::vector<std::string> completedDeliveryIds_;
};
