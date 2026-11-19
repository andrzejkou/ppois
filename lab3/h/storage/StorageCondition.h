#pragma once

#include <string>
#include <vector>

class StorageCondition {
public:
    StorageCondition(std::string conditionCode, std::string description, int quantity = 0, double amount = 0.0);

    bool isSatisfiedBy() const;
    bool requiresCooling(int delta);

    const std::string& conditionCode() const noexcept;
    const std::string& description() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string conditionCode_{};
    std::string description_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
