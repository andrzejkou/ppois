#pragma once

#include <string>
#include <vector>

class SalesOrder;
class Shipment {
public:
    Shipment(std::string shipmentId, std::string orderId, int quantity = 0, double amount = 0.0);

    bool dispatch() const;
    bool markDelivered(int delta);

    const std::string& shipmentId() const noexcept;
    const std::string& orderId() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool belongsTo(const SalesOrder& order) const;
private:
    std::string shipmentId_{};
    std::string orderId_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
