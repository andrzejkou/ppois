#include "TransferRequest.h"
#include "storage/Warehouse.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

TransferRequest::TransferRequest(std::string transferId, std::string sourceWarehouse, int quantity, double amount)
    : transferId_(std::move(transferId)), sourceWarehouse_(std::move(sourceWarehouse)),
      quantity_(quantity), amount_(amount) {
    if (transferId_.empty() || sourceWarehouse_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating TransferRequest");
}

bool TransferRequest::approveTransfer() const {
    return active_ && !transferId_.empty();
}

bool TransferRequest::calculateTransferCost(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& TransferRequest::transferId() const noexcept { return transferId_; }
const std::string& TransferRequest::sourceWarehouse() const noexcept { return sourceWarehouse_; }
int TransferRequest::quantity() const noexcept { return quantity_; }
double TransferRequest::amount() const noexcept { return amount_; }
bool TransferRequest::active() const noexcept { return active_; }

void TransferRequest::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool TransferRequest::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t TransferRequest::relatedCount() const noexcept { return relatedIds_.size(); }

bool TransferRequest::setDestination(const Warehouse& destination) {
    if (!active_ || !destination.active() || destination.code().empty() ||
        destination.code() == sourceWarehouse_) return false;
    destinationWarehouse_ = destination.code();
    return true;
}
bool TransferRequest::connects(const Warehouse& source, const Warehouse& destination) const {
    return active_ && source.active() && destination.active() &&
           source.code() == sourceWarehouse_ && destination.code() == destinationWarehouse_;
}
const std::string& TransferRequest::destinationWarehouse() const noexcept { return destinationWarehouse_; }
