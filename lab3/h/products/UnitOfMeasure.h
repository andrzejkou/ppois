#pragma once

#include <string>
#include <vector>

class UnitOfMeasure {
public:
    UnitOfMeasure(std::string unitCode, std::string symbol, int quantity = 0, double amount = 0.0);

    bool convertToBase() const;
    bool isCompatible(int delta);

    const std::string& unitCode() const noexcept;
    const std::string& symbol() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string unitCode_{};
    std::string symbol_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
