#include "StockAdjustment.h"
#include "inventory/StockBalance.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

StockAdjustment::StockAdjustment(std::string adjustmentId, std::string reason, int quantity, double amount)
    : adjustmentId_(std::move(adjustmentId)), reason_(std::move(reason)),
      quantity_(quantity), amount_(amount) {
    if (adjustmentId_.empty() || reason_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating StockAdjustment");
}

bool StockAdjustment::applyAdjustment() const {
    return active_ && !adjustmentId_.empty();
}

bool StockAdjustment::requiresApproval(int approvalThreshold) const {
    return active_ && approvalThreshold >= 0 && quantity_ >= approvalThreshold;
}

const std::string& StockAdjustment::adjustmentId() const noexcept { return adjustmentId_; }
const std::string& StockAdjustment::reason() const noexcept { return reason_; }
int StockAdjustment::quantity() const noexcept { return quantity_; }
double StockAdjustment::amount() const noexcept { return amount_; }
bool StockAdjustment::active() const noexcept { return active_; }

void StockAdjustment::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool StockAdjustment::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t StockAdjustment::relatedCount() const noexcept { return relatedIds_.size(); }

bool StockAdjustment::applyTo(StockBalance& balance, int delta) const { return active_ && delta != 0 && balance.reconcile(delta); }

bool StockAdjustment::isMaterial(int approvalThreshold) const noexcept { return approvalThreshold >= 0 && quantity_ >= approvalThreshold; }
