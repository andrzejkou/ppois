#pragma once

#include <string>
#include <vector>

class SalesOrder;
class PackingTask {
public:
    PackingTask(std::string taskId, std::string orderId, int quantity = 0, double amount = 0.0);

    double calculatePackageCount() const;
    bool completePacking(int delta);

    const std::string& taskId() const noexcept;
    const std::string& orderId() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool belongsTo(const SalesOrder& order) const;
    bool requiresOversizePackaging(double threshold) const noexcept;
private:
    std::string taskId_{};
    std::string orderId_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
