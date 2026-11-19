#pragma once

#include <string>
#include <vector>

class DeliveryNote {
public:
    DeliveryNote(std::string noteNumber, std::string supplierId, int quantity = 0, double amount = 0.0);

    bool validateAgainstOrder() const;
    bool markReceived(int delta);

    const std::string& noteNumber() const noexcept;
    const std::string& supplierId() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string noteNumber_{};
    std::string supplierId_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
