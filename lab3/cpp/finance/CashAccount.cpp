#include "CashAccount.h"
#include "finance/Payment.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

CashAccount::CashAccount(std::string accountId, std::string currency, int quantity, double amount)
    : accountId_(std::move(accountId)), currency_(std::move(currency)),
      quantity_(quantity), amount_(amount) {
    if (accountId_.empty() || currency_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating CashAccount");
}

bool CashAccount::deposit() const {
    return active_ && !accountId_.empty();
}

bool CashAccount::withdraw(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& CashAccount::accountId() const noexcept { return accountId_; }
const std::string& CashAccount::currency() const noexcept { return currency_; }
int CashAccount::quantity() const noexcept { return quantity_; }
double CashAccount::amount() const noexcept { return amount_; }
bool CashAccount::active() const noexcept { return active_; }

void CashAccount::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool CashAccount::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t CashAccount::relatedCount() const noexcept { return relatedIds_.size(); }

bool CashAccount::recordPayment(const Payment& payment) {
    if (!active_ || !payment.processPayment() || payment.paymentId().empty() ||
        hasRelatedId(payment.paymentId()) || payment.amount() <= 0.0) return false;
    if (!creditMoney(payment.amount())) return false;
    addRelatedId(payment.paymentId());
    return true;
}

bool CashAccount::credit(int amount) { if (!active_ || amount <= 0) return false; quantity_ += amount; return true; }
bool CashAccount::creditMoney(double amount) {
    if (!active_ || amount <= 0.0) return false;
    amount_ += amount;
    return true;
}
bool CashAccount::withdrawMoney(double amount) {
    if (!active_ || amount <= 0.0 || amount > amount_) return false;
    amount_ -= amount;
    return true;
}
