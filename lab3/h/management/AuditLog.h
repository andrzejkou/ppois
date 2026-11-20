#pragma once

#include <string>
#include <vector>

class Employee;
class AuditLog {
public:
    AuditLog(std::string eventId, std::string actorId, int quantity = 0, double amount = 0.0);

    bool recordEvent() const;
    bool countEventsByActor(int delta);

    const std::string& eventId() const noexcept;
    const std::string& actorId() const noexcept;
    int quantity() const noexcept;
    double amount() const noexcept;
    bool active() const noexcept;
    void addRelatedId(const std::string& relatedId);
    bool hasRelatedId(const std::string& relatedId) const;
    std::size_t relatedCount() const noexcept;

    bool records(const Employee& employee) const;
    bool belongsToActor(const std::string& employeeId) const noexcept;
private:
    std::string eventId_{};
    std::string actorId_{};
    int quantity_{0};
    double amount_{0.0};
    bool active_{true};
    std::vector<std::string> relatedIds_{};

};
