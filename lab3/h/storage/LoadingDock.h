#pragma once

#include <string>
#include <vector>

class Delivery;
class LoadingDock {
public:
    LoadingDock(std::string dockCode, std::string status, int quantity = 0, double amount = 0.0);

    bool openDock() const;
    bool closeDock(int delta);

    const std::string& dockCode() const noexcept;
    const std::string& status() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool scheduleDelivery(const Delivery& delivery);
    bool isOccupied() const noexcept;
private:
    std::string dockCode_{};
    std::string status_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
