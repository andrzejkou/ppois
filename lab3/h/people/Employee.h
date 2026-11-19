#pragma once

#include <string>
#include <vector>

class Employee {
public:
    Employee(std::string employeeId, std::string fullName, int quantity = 0, double amount = 0.0);

    bool validateIdentity() const;
    bool changeRole(int delta);

    const std::string& employeeId() const noexcept;
    const std::string& fullName() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string employeeId_{};
    std::string fullName_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
