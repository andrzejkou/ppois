#pragma once
#include <string>
class StockLot;
class QualityInspector {
public:
    QualityInspector(std::string employeeId, std::string certificateCode,
                     std::string certificateExpiry);
    bool certificateValidOn(const std::string& date) const noexcept;
    bool inspectLot(StockLot& lot, bool passed);
    bool approveQuality() const noexcept;
    std::size_t approvedLots() const noexcept;
    std::size_t rejectedLots() const noexcept;
    const std::string& employeeId() const noexcept;
    const std::string& certificateCode() const noexcept;
private:
    std::string employeeId_;
    std::string certificateCode_;
    std::string certificateExpiry_;
    std::size_t approvedLots_{0};
    std::size_t rejectedLots_{0};
};
