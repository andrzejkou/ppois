#pragma once

#include <string>
#include <vector>

class Delivery;
class Transport {
public:
    Transport(std::string transportId, std::string registrationNumber, int quantity = 0, double amount = 0.0);

    bool isAvailable() const;
    bool calculateCapacity(int delta);

    const std::string& transportId() const noexcept;
    const std::string& registrationNumber() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool canCarry(const Delivery& delivery) const;
    bool needsService(int serviceInterval) const noexcept;
private:
    std::string transportId_{};
    std::string registrationNumber_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
