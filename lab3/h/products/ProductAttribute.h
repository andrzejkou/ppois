#pragma once

#include <string>
#include <vector>

class ProductAttribute {
public:
    ProductAttribute(std::string attributeCode, std::string value, int quantity = 0, double amount = 0.0);

    bool validateValue() const;
    bool normalizeValue(int delta);

    const std::string& attributeCode() const noexcept;
    const std::string& value() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string attributeCode_{};
    std::string value_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
