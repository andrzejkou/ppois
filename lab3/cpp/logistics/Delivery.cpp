#include "Delivery.h"
#include "orders/Shipment.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Delivery::Delivery(std::string deliveryId, std::string shipmentId, int quantity, double amount)
    : deliveryId_(std::move(deliveryId)), shipmentId_(std::move(shipmentId)),
      quantity_(quantity), amount_(amount) {
    if (deliveryId_.empty() || shipmentId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Delivery");
}

bool Delivery::confirmDelivery() const {
    return active_ && !deliveryId_.empty();
}

bool Delivery::recordFailure(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Delivery::deliveryId() const noexcept { return deliveryId_; }
const std::string& Delivery::shipmentId() const noexcept { return shipmentId_; }
int Delivery::quantity() const noexcept { return quantity_; }
double Delivery::amount() const noexcept { return amount_; }
bool Delivery::active() const noexcept { return active_; }

void Delivery::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Delivery::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Delivery::relatedCount() const noexcept { return relatedIds_.size(); }

bool Delivery::refersTo(const Shipment& shipment) const { return active_ && shipment.active() && shipment.shipmentId() == shipmentId_; }

bool Delivery::isOverdue(int plannedDays, int actualDays) const noexcept { return plannedDays >= 0 && actualDays > plannedDays; }
