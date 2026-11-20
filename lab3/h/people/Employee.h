#pragma once
#include <string>
#include <vector>

class Employee {
public:
    Employee(std::string employeeId, std::string fullName, std::string position,
             double hourlyRate);
    bool validateIdentity() const noexcept;
    bool changePosition(const std::string& newPosition);
    bool changeHourlyRate(double newRate) noexcept;
    void deactivate() noexcept;
    void addSkill(const std::string& skill);
    bool hasSkill(const std::string& skill) const;
    const std::string& employeeId() const noexcept;
    const std::string& fullName() const noexcept;
    const std::string& position() const noexcept;
    double hourlyRate() const noexcept;
    bool active() const noexcept;
    const std::vector<std::string>& skills() const noexcept;
private:
    std::string employeeId_;
    std::string fullName_;
    std::string position_;
    double hourlyRate_{0.0};
    bool active_{true};
    std::vector<std::string> skills_;
};
