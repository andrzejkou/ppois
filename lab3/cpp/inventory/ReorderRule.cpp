#include "ReorderRule.h"
#include "inventory/StockItem.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

ReorderRule::ReorderRule(std::string ruleId, std::string productSku, int quantity, double amount)
    : ruleId_(std::move(ruleId)), productSku_(std::move(productSku)),
      quantity_(quantity), amount_(amount) {
    if (ruleId_.empty() || productSku_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating ReorderRule");
}

bool ReorderRule::shouldReorder() const {
    return active_ && !ruleId_.empty() && quantity_ > 0;
}

bool ReorderRule::calculateOrderQuantity(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& ReorderRule::ruleId() const noexcept { return ruleId_; }
const std::string& ReorderRule::productSku() const noexcept { return productSku_; }
int ReorderRule::quantity() const noexcept { return quantity_; }
double ReorderRule::amount() const noexcept { return amount_; }
bool ReorderRule::active() const noexcept { return active_; }

void ReorderRule::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool ReorderRule::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t ReorderRule::relatedCount() const noexcept { return relatedIds_.size(); }

bool ReorderRule::shouldReorder(const StockItem& item) const { return appliesTo(item) && item.available() <= quantity_; }

bool ReorderRule::appliesTo(const StockItem& item) const { return active_ && item.sku() == productSku_; }

int ReorderRule::shortageAgainst(const StockItem& item) const noexcept { return item.sku() == productSku_ ? (quantity_ > item.available() ? quantity_ - item.available() : 0) : 0; }
