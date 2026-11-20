#include "Storekeeper.h"
#include "exceptions/WarehouseException.h"
#include "orders/PickingTask.h"
#include "storage/StorageZone.h"
#include <algorithm>
#include <utility>
Storekeeper::Storekeeper(std::string employeeId, std::string zoneCode,
                         std::size_t taskLimit)
    : employeeId_(std::move(employeeId)), zoneCode_(std::move(zoneCode)),
      taskLimit_(taskLimit) {
  if (employeeId_.empty() || zoneCode_.empty() || taskLimit_ == 0)
    throw WarehouseException("Invalid storekeeper assignment");
}
bool Storekeeper::assignZone(const StorageZone &zone) {
  if (zone.code() != zoneCode_ || zone.capacity() <= 0)
    return false;
  return true;
}
bool Storekeeper::assignPickingTask(const PickingTask &task) {
  if (!canAcceptTask() || task.taskId().empty() ||
      std::find(assignedTaskIds_.begin(), assignedTaskIds_.end(),
                task.taskId()) != assignedTaskIds_.end())
    return false;
  assignedTaskIds_.push_back(task.taskId());
  return true;
}
bool Storekeeper::completeTask(const std::string &taskId) {
  auto it = std::find(assignedTaskIds_.begin(), assignedTaskIds_.end(), taskId);
  if (it == assignedTaskIds_.end())
    return false;
  assignedTaskIds_.erase(it);
  return true;
}
std::size_t Storekeeper::countAssignedTasks() const noexcept {
  return assignedTaskIds_.size();
}
bool Storekeeper::canAcceptTask() const noexcept {
  return assignedTaskIds_.size() < taskLimit_;
}
const std::string &Storekeeper::employeeId() const noexcept {
  return employeeId_;
}
const std::string &Storekeeper::zoneCode() const noexcept { return zoneCode_; }
