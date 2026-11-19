#include "StockMovement.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StockMovement::StockMovement(std::string movementId, std::string productSku, int quantity, double amount)
    : movementId_(std::move(movementId)), productSku_(std::move(productSku)),
      quantity_(quantity), amount_(amount) {
    if (movementId_.empty() || productSku_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StockMovement");
}

double StockMovement::calculateSignedQuantity() const {
    return amount_ * static_cast<double>(quantity_);
}

bool StockMovement::isTransfer(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& StockMovement::movementId() const noexcept { return movementId_; }
const std::string& StockMovement::productSku() const noexcept { return productSku_; }
int StockMovement::quantity() const noexcept { return quantity_; }
double StockMovement::amount() const noexcept { return amount_; }
bool StockMovement::active() const noexcept { return active_; }

void StockMovement::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StockMovement::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StockMovement::relatedCount() const noexcept { return relatedIds_.size(); }
