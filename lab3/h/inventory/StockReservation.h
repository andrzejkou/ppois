#pragma once

#include <string>
#include <vector>

class StockReservation {
public:
    StockReservation(std::string reservationId, std::string orderId, int quantity = 0, double amount = 0.0);

    bool reserve() const;
    bool release(int delta);

    const std::string& reservationId() const noexcept;
    const std::string& orderId() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string reservationId_{};
    std::string orderId_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
