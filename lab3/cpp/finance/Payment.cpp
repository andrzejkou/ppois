#include "Payment.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Payment::Payment(std::string paymentId, std::string purpose, int quantity, double amount)
    : paymentId_(std::move(paymentId)), purpose_(std::move(purpose)),
      quantity_(quantity), amount_(amount) {
    if (paymentId_.empty() || purpose_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Payment");
}

bool Payment::processPayment() const {
    return active_ && !paymentId_.empty();
}

bool Payment::refund(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& Payment::paymentId() const noexcept { return paymentId_; }
const std::string& Payment::purpose() const noexcept { return purpose_; }
int Payment::quantity() const noexcept { return quantity_; }
double Payment::amount() const noexcept { return amount_; }
bool Payment::active() const noexcept { return active_; }

void Payment::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Payment::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Payment::relatedCount() const noexcept { return relatedIds_.size(); }
