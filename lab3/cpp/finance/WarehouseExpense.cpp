#include "WarehouseExpense.h"
#include "finance/FinancialReport.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

WarehouseExpense::WarehouseExpense(std::string expenseId, std::string category, int quantity, double amount)
    : expenseId_(std::move(expenseId)), category_(std::move(category)),
      quantity_(quantity), amount_(amount) {
    if (expenseId_.empty() || category_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating WarehouseExpense");
}

bool WarehouseExpense::approveExpense() const {
    return active_ && !expenseId_.empty();
}

bool WarehouseExpense::approveBy(const std::string& managerId) {
    if (!active_ || managerId.empty() || !approvedBy_.empty()) return false;
    approvedBy_ = managerId; return true;
}

const std::string& WarehouseExpense::approvedBy() const noexcept { return approvedBy_; }

bool WarehouseExpense::isWithinBudget(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& WarehouseExpense::expenseId() const noexcept { return expenseId_; }
const std::string& WarehouseExpense::category() const noexcept { return category_; }
int WarehouseExpense::quantity() const noexcept { return quantity_; }
double WarehouseExpense::amount() const noexcept { return amount_; }
bool WarehouseExpense::active() const noexcept { return active_; }

void WarehouseExpense::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool WarehouseExpense::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t WarehouseExpense::relatedCount() const noexcept { return relatedIds_.size(); }

bool WarehouseExpense::attachTo(FinancialReport& report) const { if (!active_ || approvedBy_.empty()) return false; report.addRelatedId(expenseId_); return report.hasRelatedId(expenseId_); }

bool WarehouseExpense::exceedsLimit(double limit) const noexcept { return limit >= 0.0 && amount_ > limit; }
