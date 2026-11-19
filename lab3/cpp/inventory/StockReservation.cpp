#include "StockReservation.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StockReservation::StockReservation(std::string reservationId, std::string orderId, int quantity, double amount)
    : reservationId_(std::move(reservationId)), orderId_(std::move(orderId)),
      quantity_(quantity), amount_(amount) {
    if (reservationId_.empty() || orderId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StockReservation");
}

bool StockReservation::reserve() const {
    return active_ && !reservationId_.empty();
}

bool StockReservation::release(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& StockReservation::reservationId() const noexcept { return reservationId_; }
const std::string& StockReservation::orderId() const noexcept { return orderId_; }
int StockReservation::quantity() const noexcept { return quantity_; }
double StockReservation::amount() const noexcept { return amount_; }
bool StockReservation::active() const noexcept { return active_; }

void StockReservation::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StockReservation::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StockReservation::relatedCount() const noexcept { return relatedIds_.size(); }
