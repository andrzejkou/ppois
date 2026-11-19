#pragma once

#include <string>
#include <vector>

class Supplier {
public:
    Supplier(std::string supplierId, std::string companyName, int quantity = 0, double amount = 0.0);

    double rateReliability() const;
    bool canSupply(int delta);

    const std::string& supplierId() const noexcept;
    const std::string& companyName() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string supplierId_{};
    std::string companyName_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
