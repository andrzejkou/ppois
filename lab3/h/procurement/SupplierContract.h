#pragma once

#include <string>
#include <vector>

class SupplierContract {
public:
    SupplierContract(std::string contractId, std::string supplierId, int quantity = 0, double amount = 0.0);

    bool isValidOn() const;
    bool calculatePenalty(int delta);

    const std::string& contractId() const noexcept;
    const std::string& supplierId() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string contractId_{};
    std::string supplierId_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
