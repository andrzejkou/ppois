#pragma once

#include <string>
#include <vector>

class ProductBundle {
public:
    ProductBundle(std::string bundleCode, std::string name, int quantity = 0, double amount = 0.0);

    double calculateBundlePrice() const;
    bool validateComponents(int delta);

    const std::string& bundleCode() const noexcept;
    const std::string& name() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string bundleCode_{};
    std::string name_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
