#include "CostCalculation.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

CostCalculation::CostCalculation(std::string calculationId, std::string productSku, int quantity, double amount)
    : calculationId_(std::move(calculationId)), productSku_(std::move(productSku)),
      quantity_(quantity), amount_(amount) {
    if (calculationId_.empty() || productSku_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating CostCalculation");
}

double CostCalculation::calculateAverageCost() const {
    return amount_ * static_cast<double>(quantity_);
}

bool CostCalculation::calculateWeightedCost(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& CostCalculation::calculationId() const noexcept { return calculationId_; }
const std::string& CostCalculation::productSku() const noexcept { return productSku_; }
int CostCalculation::quantity() const noexcept { return quantity_; }
double CostCalculation::amount() const noexcept { return amount_; }
bool CostCalculation::active() const noexcept { return active_; }

void CostCalculation::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool CostCalculation::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t CostCalculation::relatedCount() const noexcept { return relatedIds_.size(); }
