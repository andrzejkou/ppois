#include "ReturnRequest.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

ReturnRequest::ReturnRequest(std::string returnId, std::string orderId, int quantity, double amount)
    : returnId_(std::move(returnId)), orderId_(std::move(orderId)),
      quantity_(quantity), amount_(amount) {
    if (returnId_.empty() || orderId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating ReturnRequest");
}

bool ReturnRequest::approveReturn() const {
    return active_ && !returnId_.empty();
}

bool ReturnRequest::calculateRefund(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& ReturnRequest::returnId() const noexcept { return returnId_; }
const std::string& ReturnRequest::orderId() const noexcept { return orderId_; }
int ReturnRequest::quantity() const noexcept { return quantity_; }
double ReturnRequest::amount() const noexcept { return amount_; }
bool ReturnRequest::active() const noexcept { return active_; }

void ReturnRequest::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool ReturnRequest::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t ReturnRequest::relatedCount() const noexcept { return relatedIds_.size(); }

bool ReturnRequest::isWithinQuantityLimit(int maximumQuantity) const noexcept { return active_ && maximumQuantity >= 0 && quantity_ <= maximumQuantity; }
