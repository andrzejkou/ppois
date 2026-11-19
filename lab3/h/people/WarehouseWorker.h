#pragma once

#include <string>
#include <vector>

class WarehouseWorker {
public:
    WarehouseWorker(std::string employeeId, std::string shiftCode, int quantity = 0, double amount = 0.0);

    bool startShift() const;
    bool finishShift(int delta);

    const std::string& employeeId() const noexcept;
    const std::string& shiftCode() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string employeeId_{};
    std::string shiftCode_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
