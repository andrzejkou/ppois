#include "DeliveryNote.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

DeliveryNote::DeliveryNote(std::string noteNumber, std::string supplierId, int quantity, double amount)
    : noteNumber_(std::move(noteNumber)), supplierId_(std::move(supplierId)),
      quantity_(quantity), amount_(amount) {
    if (noteNumber_.empty() || supplierId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating DeliveryNote");
}

bool DeliveryNote::validateAgainstOrder() const {
    return active_ && !noteNumber_ .empty() && quantity_ >= 0;
}

bool DeliveryNote::markReceived(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& DeliveryNote::noteNumber() const noexcept { return noteNumber_; }
const std::string& DeliveryNote::supplierId() const noexcept { return supplierId_; }
int DeliveryNote::quantity() const noexcept { return quantity_; }
double DeliveryNote::amount() const noexcept { return amount_; }
bool DeliveryNote::active() const noexcept { return active_; }

void DeliveryNote::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool DeliveryNote::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t DeliveryNote::relatedCount() const noexcept { return relatedIds_.size(); }
