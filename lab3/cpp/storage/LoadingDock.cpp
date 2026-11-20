#include "LoadingDock.h"
#include "logistics/Delivery.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

LoadingDock::LoadingDock(std::string dockCode, std::string status, int quantity, double amount)
    : dockCode_(std::move(dockCode)), status_(std::move(status)),
      quantity_(quantity), amount_(amount) {
    if (dockCode_.empty() || status_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating LoadingDock");
}

bool LoadingDock::openDock() const {
    return active_ && !dockCode_.empty();
}

bool LoadingDock::closeDock(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& LoadingDock::dockCode() const noexcept { return dockCode_; }
const std::string& LoadingDock::status() const noexcept { return status_; }
int LoadingDock::quantity() const noexcept { return quantity_; }
double LoadingDock::amount() const noexcept { return amount_; }
bool LoadingDock::active() const noexcept { return active_; }

void LoadingDock::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool LoadingDock::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t LoadingDock::relatedCount() const noexcept { return relatedIds_.size(); }

bool LoadingDock::scheduleDelivery(const Delivery& delivery) { if (!active_ || status_ != "open" || !delivery.active() || delivery.deliveryId().empty() || hasRelatedId(delivery.deliveryId())) return false; addRelatedId(delivery.deliveryId()); return true; }

bool LoadingDock::isOccupied() const noexcept { return quantity_ > 0 || !relatedIds_.empty(); }
