#include "QualityInspector.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

QualityInspector::QualityInspector(std::string employeeId, std::string certificateCode, int quantity, double amount)
    : employeeId_(std::move(employeeId)), certificateCode_(std::move(certificateCode)),
      quantity_(quantity), amount_(amount) {
    if (employeeId_.empty() || certificateCode_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating QualityInspector");
}

bool QualityInspector::approveQuality() const {
    return active_ && !employeeId_.empty();
}

bool QualityInspector::rejectBatch(int delta) {
    if (!active_ || delta < 0 || quantity_ < delta) return false;
    quantity_ -= delta;
    return true;
}

const std::string& QualityInspector::employeeId() const noexcept { return employeeId_; }
const std::string& QualityInspector::certificateCode() const noexcept { return certificateCode_; }
int QualityInspector::quantity() const noexcept { return quantity_; }
double QualityInspector::amount() const noexcept { return amount_; }
bool QualityInspector::active() const noexcept { return active_; }

void QualityInspector::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool QualityInspector::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t QualityInspector::relatedCount() const noexcept { return relatedIds_.size(); }
