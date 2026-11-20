#include "QualityInspector.h"
#include "inventory/StockLot.h"
#include "exceptions/WarehouseException.h"
#include <utility>
QualityInspector::QualityInspector(std::string employeeId, std::string certificateCode,
                                   std::string certificateExpiry)
    : employeeId_(std::move(employeeId)), certificateCode_(std::move(certificateCode)),
      certificateExpiry_(std::move(certificateExpiry)) {
    if (employeeId_.empty() || certificateCode_.empty() || certificateExpiry_.size() != 10)
        throw WarehouseException("Invalid quality inspector certificate");
}
bool QualityInspector::certificateValidOn(const std::string& date) const noexcept {
    return date.size() == 10 && date <= certificateExpiry_;
}
bool QualityInspector::inspectLot(StockLot& lot, bool passed) {
    if (lot.quantity() <= 0) return false;
    if (passed) { lot.releaseFromQuarantine(); ++approvedLots_; }
    else { lot.markQuarantined(); ++rejectedLots_; }
    return true;
}
bool QualityInspector::approveQuality() const noexcept { return approvedLots_ > 0; }
std::size_t QualityInspector::approvedLots() const noexcept { return approvedLots_; }
std::size_t QualityInspector::rejectedLots() const noexcept { return rejectedLots_; }
const std::string& QualityInspector::employeeId() const noexcept { return employeeId_; }
const std::string& QualityInspector::certificateCode() const noexcept { return certificateCode_; }
