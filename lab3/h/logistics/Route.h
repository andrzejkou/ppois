#pragma once

#include <string>
#include <vector>

class Route {
public:
    Route(std::string routeId, std::string destination, int quantity = 0, double amount = 0.0);

    double calculateDistance() const;
    bool estimateDuration(int delta);

    const std::string& routeId() const noexcept;
    const std::string& destination() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string routeId_{};
    std::string destination_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
