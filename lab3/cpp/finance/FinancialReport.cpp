#include "FinancialReport.h"
#include "finance/WarehouseExpense.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

FinancialReport::FinancialReport(std::string reportId, std::string period, int quantity, double amount)
    : reportId_(std::move(reportId)), period_(std::move(period)),
      quantity_(quantity), amount_(amount) {
    if (reportId_.empty() || period_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating FinancialReport");
}

double FinancialReport::calculateNetResult() const {
    return amount_;
}

bool FinancialReport::validatePeriod(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& FinancialReport::reportId() const noexcept { return reportId_; }
const std::string& FinancialReport::period() const noexcept { return period_; }
int FinancialReport::quantity() const noexcept { return quantity_; }
double FinancialReport::amount() const noexcept { return amount_; }
bool FinancialReport::active() const noexcept { return active_; }

void FinancialReport::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool FinancialReport::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t FinancialReport::relatedCount() const noexcept { return relatedIds_.size(); }

bool FinancialReport::includesExpense(const WarehouseExpense& expense) const { return active_ && hasRelatedId(expense.expenseId()); }

double FinancialReport::averageTransactionValue() const noexcept { return quantity_ > 0 ? amount_ / static_cast<double>(quantity_) : 0.0; }
