#include "PurchaseRequest.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

PurchaseRequest::PurchaseRequest(std::string requestId, std::string departmentCode, int quantity, double amount)
    : requestId_(std::move(requestId)), departmentCode_(std::move(departmentCode)),
      quantity_(quantity), amount_(amount) {
    if (requestId_.empty() || departmentCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating PurchaseRequest");
}

bool PurchaseRequest::submit() const {
    return active_ && !requestId_.empty();
}

bool PurchaseRequest::cancel(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& PurchaseRequest::requestId() const noexcept { return requestId_; }
const std::string& PurchaseRequest::departmentCode() const noexcept { return departmentCode_; }
int PurchaseRequest::quantity() const noexcept { return quantity_; }
double PurchaseRequest::amount() const noexcept { return amount_; }
bool PurchaseRequest::active() const noexcept { return active_; }

void PurchaseRequest::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool PurchaseRequest::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t PurchaseRequest::relatedCount() const noexcept { return relatedIds_.size(); }
