#pragma once

#include <string>
#include <vector>

class PurchaseRequest {
public:
    PurchaseRequest(std::string requestId, std::string departmentCode, int quantity = 0, double amount = 0.0);

    bool submit() const;
    bool cancel(int delta);

    const std::string& requestId() const noexcept;
    const std::string& departmentCode() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string requestId_{};
    std::string departmentCode_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
