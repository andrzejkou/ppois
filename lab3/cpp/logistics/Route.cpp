#include "Route.h"
#include "logistics/Transport.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Route::Route(std::string routeId, std::string destination, int quantity, double amount)
    : routeId_(std::move(routeId)), destination_(std::move(destination)),
      quantity_(quantity), amount_(amount) {
    if (routeId_.empty() || destination_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Route");
}

double Route::calculateDistance() const {
    return amount_ * static_cast<double>(quantity_);
}

bool Route::estimateDuration(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Route::routeId() const noexcept { return routeId_; }
const std::string& Route::destination() const noexcept { return destination_; }
int Route::quantity() const noexcept { return quantity_; }
double Route::amount() const noexcept { return amount_; }
bool Route::active() const noexcept { return active_; }

void Route::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Route::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Route::relatedCount() const noexcept { return relatedIds_.size(); }

bool Route::supports(const Transport& transport) const { return active_ && transport.isAvailable() && !destination_.empty(); }

bool Route::isWithinDistanceLimit(double maximumDistance) const noexcept { return maximumDistance >= 0.0 && amount_ <= maximumDistance; }
