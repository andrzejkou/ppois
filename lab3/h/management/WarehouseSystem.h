#pragma once

#include <string>
#include <vector>

class WarehouseSystem {
public:
    WarehouseSystem(std::string systemId, std::string systemName, int quantity = 0, double amount = 0.0);

    bool registerWarehouse() const;
    bool calculateNetworkStock(int delta);

    const std::string& systemId() const noexcept;
    const std::string& systemName() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string systemId_{};
    std::string systemName_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
