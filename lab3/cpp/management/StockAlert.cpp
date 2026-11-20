#include "StockAlert.h"
#include "inventory/ReorderRule.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StockAlert::StockAlert(std::string alertId, std::string productSku, int quantity, double amount)
    : alertId_(std::move(alertId)), productSku_(std::move(productSku)),
      quantity_(quantity), amount_(amount) {
    if (alertId_.empty() || productSku_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StockAlert");
}

bool StockAlert::triggerIfLow() const {
    return active_ && !alertId_.empty();
}

bool StockAlert::acknowledge(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& StockAlert::alertId() const noexcept { return alertId_; }
const std::string& StockAlert::productSku() const noexcept { return productSku_; }
int StockAlert::quantity() const noexcept { return quantity_; }
double StockAlert::amount() const noexcept { return amount_; }
bool StockAlert::active() const noexcept { return active_; }

void StockAlert::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StockAlert::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StockAlert::relatedCount() const noexcept { return relatedIds_.size(); }

bool StockAlert::follows(const ReorderRule& rule) const { return active_ && rule.active() && rule.productSku() == productSku_; }

bool StockAlert::isCritical(int criticalLevel) const noexcept { return active_ && criticalLevel >= 0 && quantity_ <= criticalLevel; }
