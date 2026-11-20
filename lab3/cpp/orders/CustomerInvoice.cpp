#include "CustomerInvoice.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

CustomerInvoice::CustomerInvoice(std::string invoiceId, std::string customerId, int quantity, double amount)
    : invoiceId_(std::move(invoiceId)), customerId_(std::move(customerId)),
      quantity_(quantity), amount_(amount) {
    if (invoiceId_.empty() || customerId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating CustomerInvoice");
}

double CustomerInvoice::calculateDue() const {
    return amount_ * static_cast<double>(quantity_);
}

bool CustomerInvoice::markPaid(double paymentAmount) {
    if (!active_ || paymentAmount <= 0.0 || paymentAmount > outstandingBalance()) return false;
    paidAmount_ += paymentAmount;
    return true;
}

const std::string& CustomerInvoice::invoiceId() const noexcept { return invoiceId_; }
const std::string& CustomerInvoice::customerId() const noexcept { return customerId_; }
int CustomerInvoice::quantity() const noexcept { return quantity_; }
double CustomerInvoice::amount() const noexcept { return amount_; }
bool CustomerInvoice::active() const noexcept { return active_; }

void CustomerInvoice::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool CustomerInvoice::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t CustomerInvoice::relatedCount() const noexcept { return relatedIds_.size(); }

bool CustomerInvoice::hasOutstandingBalance() const noexcept { return active_ && paidAmount_ < calculateDue(); }
double CustomerInvoice::outstandingBalance() const noexcept { const double due = calculateDue() - paidAmount_; return due > 0.0 ? due : 0.0; }
