#include "KPIReport.h"
#include "inventory/Inventory.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

KPIReport::KPIReport(std::string reportId, std::string period, int quantity, double amount)
    : reportId_(std::move(reportId)), period_(std::move(period)),
      quantity_(quantity), amount_(amount) {
    if (reportId_.empty() || period_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating KPIReport");
}

double KPIReport::calculateOrderFulfillmentRate() const {
    return amount_ * static_cast<double>(quantity_);
}

bool KPIReport::calculateInventoryTurnover(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& KPIReport::reportId() const noexcept { return reportId_; }
const std::string& KPIReport::period() const noexcept { return period_; }
int KPIReport::quantity() const noexcept { return quantity_; }
double KPIReport::amount() const noexcept { return amount_; }
bool KPIReport::active() const noexcept { return active_; }

void KPIReport::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool KPIReport::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t KPIReport::relatedCount() const noexcept { return relatedIds_.size(); }

bool KPIReport::evaluates(const Inventory& inventory) const { return active_ && inventory.status() == InventoryStatus::Completed; }

double KPIReport::averageOrdersPerPeriod() const noexcept { return amount_ > 0.0 ? static_cast<double>(quantity_) / amount_ : 0.0; }
