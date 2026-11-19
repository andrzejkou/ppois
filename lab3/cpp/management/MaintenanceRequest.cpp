#include "MaintenanceRequest.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

MaintenanceRequest::MaintenanceRequest(std::string requestId, std::string equipmentCode, int quantity, double amount)
    : requestId_(std::move(requestId)), equipmentCode_(std::move(equipmentCode)),
      quantity_(quantity), amount_(amount) {
    if (requestId_.empty() || equipmentCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating MaintenanceRequest");
}

bool MaintenanceRequest::scheduleMaintenance() const {
    return active_ && !requestId_.empty();
}

bool MaintenanceRequest::closeRequest(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& MaintenanceRequest::requestId() const noexcept { return requestId_; }
const std::string& MaintenanceRequest::equipmentCode() const noexcept { return equipmentCode_; }
int MaintenanceRequest::quantity() const noexcept { return quantity_; }
double MaintenanceRequest::amount() const noexcept { return amount_; }
bool MaintenanceRequest::active() const noexcept { return active_; }

void MaintenanceRequest::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool MaintenanceRequest::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t MaintenanceRequest::relatedCount() const noexcept { return relatedIds_.size(); }
