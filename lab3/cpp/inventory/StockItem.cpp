#include "StockItem.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StockItem::StockItem(std::string stockId, std::string productSku, int quantity, double amount)
    : stockId_(std::move(stockId)), productSku_(std::move(productSku)),
      quantity_(quantity), amount_(amount) {
    if (stockId_.empty() || productSku_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StockItem");
}

bool StockItem::increaseStock() const {
    return active_ && !stockId_.empty();
}

bool StockItem::decreaseStock(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& StockItem::stockId() const noexcept { return stockId_; }
const std::string& StockItem::productSku() const noexcept { return productSku_; }
int StockItem::quantity() const noexcept { return quantity_; }
double StockItem::amount() const noexcept { return amount_; }
bool StockItem::active() const noexcept { return active_; }

void StockItem::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StockItem::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StockItem::relatedCount() const noexcept { return relatedIds_.size(); }
