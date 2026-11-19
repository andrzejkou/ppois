#include "StockLot.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StockLot::StockLot(std::string lotNumber, std::string productSku, int quantity, double amount)
    : lotNumber_(std::move(lotNumber)), productSku_(std::move(productSku)),
      quantity_(quantity), amount_(amount) {
    if (lotNumber_.empty() || productSku_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StockLot");
}

bool StockLot::isExpired() const {
    return active_ && !lotNumber_ .empty() && quantity_ >= 0;
}

bool StockLot::remainingShelfLife(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& StockLot::lotNumber() const noexcept { return lotNumber_; }
const std::string& StockLot::productSku() const noexcept { return productSku_; }
int StockLot::quantity() const noexcept { return quantity_; }
double StockLot::amount() const noexcept { return amount_; }
bool StockLot::active() const noexcept { return active_; }

void StockLot::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StockLot::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StockLot::relatedCount() const noexcept { return relatedIds_.size(); }
