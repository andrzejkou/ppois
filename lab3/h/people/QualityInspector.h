#pragma once

#include <string>
#include <vector>

class QualityInspector {
public:
    QualityInspector(std::string employeeId, std::string certificateCode, int quantity = 0, double amount = 0.0);

    bool approveQuality() const;
    bool rejectBatch(int delta);

    const std::string& employeeId() const noexcept;
    const std::string& certificateCode() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string employeeId_{};
    std::string certificateCode_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
