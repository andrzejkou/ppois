#pragma once

#include <string>
#include <vector>

class WarehouseExpense;
class FinancialReport {
public:
    FinancialReport(std::string reportId, std::string period, int quantity = 0, double amount = 0.0);

    double calculateNetResult() const;
    bool validatePeriod(int delta);

    const std::string& reportId() const noexcept;
    const std::string& period() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool includesExpense(const WarehouseExpense& expense) const;
    double averageTransactionValue() const noexcept;
private:
    std::string reportId_{};
    std::string period_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
