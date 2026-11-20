#pragma once

#include <string>
#include <vector>

class Shelf;
class StorageBin {
public:
    StorageBin(std::string binCode, std::string shelfCode, int quantity = 0, double amount = 0.0);

    double calculateFreeCapacity() const;
    bool canFit(int delta);

    const std::string& binCode() const noexcept;
    const std::string& shelfCode() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool belongsTo(const Shelf& shelf) const;
    bool isEmpty() const noexcept;
private:
    std::string binCode_{};
    std::string shelfCode_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
