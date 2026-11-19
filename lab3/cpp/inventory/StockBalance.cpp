#include "StockBalance.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StockBalance::StockBalance(std::string balanceId, std::string warehouseCode, int quantity, double amount)
    : balanceId_(std::move(balanceId)), warehouseCode_(std::move(warehouseCode)),
      quantity_(quantity), amount_(amount) {
    if (balanceId_.empty() || warehouseCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StockBalance");
}

double StockBalance::calculateAvailable() const {
    return amount_ * static_cast<double>(quantity_);
}

bool StockBalance::reconcile(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& StockBalance::balanceId() const noexcept { return balanceId_; }
const std::string& StockBalance::warehouseCode() const noexcept { return warehouseCode_; }
int StockBalance::quantity() const noexcept { return quantity_; }
double StockBalance::amount() const noexcept { return amount_; }
bool StockBalance::active() const noexcept { return active_; }

void StockBalance::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StockBalance::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StockBalance::relatedCount() const noexcept { return relatedIds_.size(); }
