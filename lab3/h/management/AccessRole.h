#pragma once

#include <string>
#include <vector>

class AccessRole {
public:
    AccessRole(std::string roleCode, std::string roleName, int quantity = 0, double amount = 0.0);

    bool grantPermission() const;
    bool revokePermission(int delta);

    const std::string& roleCode() const noexcept;
    const std::string& roleName() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

private:
    std::string roleCode_{};
    std::string roleName_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
