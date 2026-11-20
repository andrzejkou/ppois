#include "Shipment.h"
#include "orders/SalesOrder.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Shipment::Shipment(std::string shipmentId, std::string orderId, int quantity, double amount)
    : shipmentId_(std::move(shipmentId)), orderId_(std::move(orderId)),
      quantity_(quantity), amount_(amount) {
    if (shipmentId_.empty() || orderId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Shipment");
}

bool Shipment::dispatch() const {
    return active_ && !shipmentId_.empty();
}

bool Shipment::markDelivered(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Shipment::shipmentId() const noexcept { return shipmentId_; }
const std::string& Shipment::orderId() const noexcept { return orderId_; }
int Shipment::quantity() const noexcept { return quantity_; }
double Shipment::amount() const noexcept { return amount_; }
bool Shipment::active() const noexcept { return active_; }

void Shipment::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Shipment::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Shipment::relatedCount() const noexcept { return relatedIds_.size(); }

bool Shipment::belongsTo(const SalesOrder& order) const { return active_ && order.orderId() == orderId_; }
