#pragma once

#include <string>
#include <vector>

class CostCalculation {
public:
    CostCalculation(std::string calculationId, std::string productSku, int quantity = 0, double amount = 0.0);

    double calculateAverageCost() const;
    bool calculateWeightedCost(int delta);

    const std::string& calculationId() const noexcept;
    const std::string& productSku() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool addPurchase(int units, double unitCost);
private:
    std::string calculationId_{};
    std::string productSku_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
