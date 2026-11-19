#include "Transport.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Transport::Transport(std::string transportId, std::string registrationNumber, int quantity, double amount)
    : transportId_(std::move(transportId)), registrationNumber_(std::move(registrationNumber)),
      quantity_(quantity), amount_(amount) {
    if (transportId_.empty() || registrationNumber_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Transport");
}

bool Transport::isAvailable() const {
    return active_ && !transportId_ .empty() && quantity_ >= 0;
}

bool Transport::calculateCapacity(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Transport::transportId() const noexcept { return transportId_; }
const std::string& Transport::registrationNumber() const noexcept { return registrationNumber_; }
int Transport::quantity() const noexcept { return quantity_; }
double Transport::amount() const noexcept { return amount_; }
bool Transport::active() const noexcept { return active_; }

void Transport::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Transport::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Transport::relatedCount() const noexcept { return relatedIds_.size(); }
