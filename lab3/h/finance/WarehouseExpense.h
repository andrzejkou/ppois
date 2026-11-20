#pragma once

#include <string>
#include <vector>

class FinancialReport;
class WarehouseExpense {
public:
    WarehouseExpense(std::string expenseId, std::string category, int quantity = 0, double amount = 0.0);

    bool approveExpense() const;
    bool approveBy(const std::string& managerId);
    const std::string& approvedBy() const noexcept;
    bool isWithinBudget(int delta);

    const std::string& expenseId() const noexcept;
    const std::string& category() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool attachTo(FinancialReport& report) const;
    bool exceedsLimit(double limit) const noexcept;
private:
    std::string expenseId_{};
    std::string category_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};
    std::string approvedBy_{};

};
