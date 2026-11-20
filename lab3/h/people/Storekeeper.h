#pragma once
#include <string>
#include <vector>
class StorageZone;
class PickingTask;
class Storekeeper {
public:
    Storekeeper(std::string employeeId, std::string zoneCode, std::size_t taskLimit);
    bool assignZone(const StorageZone& zone);
    bool assignPickingTask(const PickingTask& task);
    bool completeTask(const std::string& taskId);
    std::size_t countAssignedTasks() const noexcept;
    bool canAcceptTask() const noexcept;
    const std::string& employeeId() const noexcept;
    const std::string& zoneCode() const noexcept;
private:
    std::string employeeId_;
    std::string zoneCode_;
    std::size_t taskLimit_{0};
    std::vector<std::string> assignedTaskIds_;
};
