#pragma once
#include <string>
#include <vector>
class PickingTask;
class PackingTask;
class WarehouseWorker {
public:
    WarehouseWorker(std::string employeeId, std::string shiftCode, double hourlyRate);
    bool startShift(const std::string& date);
    bool finishShift(const std::string& date);
    bool assignPickingTask(const PickingTask& task);
    bool assignPackingTask(const PackingTask& task);
    bool completeTask(const std::string& taskId);
    double calculateShiftPay(double hoursWorked) const noexcept;
    bool onShift() const noexcept;
    const std::string& employeeId() const noexcept;
    const std::string& shiftCode() const noexcept;
private:
    std::string employeeId_;
    std::string shiftCode_;
    double hourlyRate_{0.0};
    bool onShift_{false};
    std::string shiftStartDate_;
    std::vector<std::string> taskIds_;
};
