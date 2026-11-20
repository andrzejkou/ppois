#pragma once

#include <string>
#include <vector>
class Product;

class SerialNumber {
public:
    SerialNumber(std::string serialCode, std::string productSku, int quantity = 0, double amount = 0.0);

    bool assignProduct() const;
    bool isAssigned(int delta);

    const std::string& serialCode() const noexcept;
    const std::string& productSku() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool belongsTo(const Product& product) const;
    bool isRegistered() const noexcept;
private:
    std::string serialCode_{};
    std::string productSku_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
