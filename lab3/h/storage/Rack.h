#pragma once

#include <string>
#include <vector>

class StorageZone;
class Rack {
public:
    Rack(std::string rackCode, std::string zoneCode, int quantity = 0, double amount = 0.0);

    double calculateLoad() const;
    bool hasFreeLevel(int delta);

    const std::string& rackCode() const noexcept;
    const std::string& zoneCode() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool belongsTo(const StorageZone& zone) const;
    bool canSupport(double weight) const noexcept;
private:
    std::string rackCode_{};
    std::string zoneCode_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
