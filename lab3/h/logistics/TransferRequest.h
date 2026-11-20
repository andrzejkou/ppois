#pragma once

#include <string>
#include <vector>
class Warehouse;

class TransferRequest {
public:
    TransferRequest(std::string transferId, std::string sourceWarehouse, int quantity = 0, double amount = 0.0);

    bool approveTransfer() const;
    bool calculateTransferCost(int delta);

    const std::string& transferId() const noexcept;
    const std::string& sourceWarehouse() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool setDestination(const Warehouse& destination);
    bool connects(const Warehouse& source, const Warehouse& destination) const;
    const std::string& destinationWarehouse() const noexcept;
private:
    std::string transferId_{};
    std::string sourceWarehouse_{};
    std::string destinationWarehouse_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
