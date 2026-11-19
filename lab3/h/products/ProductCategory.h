#pragma once

#include <string>
#include <vector>

class ProductCategory {
public:
    ProductCategory(std::string categoryCode, std::string name, int quantity = 0, double amount = 0.0);

    bool addProduct() const;
    bool removeProduct(int delta);

    const std::string& categoryCode() const noexcept;
    const std::string& name() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string categoryCode_{};
    std::string name_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
