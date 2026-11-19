#pragma once

#include <string>
#include <vector>

class ColdStorage {
public:
    ColdStorage(std::string storageCode, std::string temperature, int quantity = 0, double amount = 0.0);

    bool validateTemperature() const;
    bool estimateEnergyUse(int delta);

    const std::string& storageCode() const noexcept;
    const std::string& temperature() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string storageCode_{};
    std::string temperature_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
