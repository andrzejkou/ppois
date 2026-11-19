#pragma once

#include <string>
#include <vector>

class StorageZone {
public:
    StorageZone(std::string zoneCode, std::string temperatureMode, int quantity = 0, double amount = 0.0);

    bool isSuitableFor() const;
    bool calculateOccupancy(int delta);

    const std::string& zoneCode() const noexcept;
    const std::string& temperatureMode() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string zoneCode_{};
    std::string temperatureMode_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
