#pragma once

#include <string>
#include <vector>

class Rack;
class Shelf {
public:
    Shelf(std::string shelfCode, std::string rackCode, int quantity = 0, double amount = 0.0);

    double calculateFreeVolume() const;
    bool canStore(int delta);

    const std::string& shelfCode() const noexcept;
    const std::string& rackCode() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool belongsTo(const Rack& rack) const;
    bool hasCapacityFor(double volume) const noexcept;
private:
    std::string shelfCode_{};
    std::string rackCode_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
