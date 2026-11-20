#include "WarehouseWorker.h"
#include "orders/PickingTask.h"
#include "orders/PackingTask.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
WarehouseWorker::WarehouseWorker(std::string employeeId, std::string shiftCode, double hourlyRate)
    : employeeId_(std::move(employeeId)), shiftCode_(std::move(shiftCode)), hourlyRate_(hourlyRate) {
    if (employeeId_.empty() || shiftCode_.empty() || hourlyRate_ < 0.0)
        throw WarehouseException("Invalid warehouse worker data");
}
bool WarehouseWorker::startShift(const std::string& date) {
    if (onShift_ || date.empty()) return false;
    onShift_ = true; shiftStartDate_ = date; return true;
}
bool WarehouseWorker::finishShift(const std::string& date) {
    if (!onShift_ || date < shiftStartDate_ || !taskIds_.empty()) return false;
    onShift_ = false; shiftStartDate_.clear(); return true;
}
bool WarehouseWorker::assignPickingTask(const PickingTask& task) {
    if (!onShift_ || task.taskId().empty() ||
        std::find(taskIds_.begin(), taskIds_.end(), task.taskId()) != taskIds_.end()) return false;
    taskIds_.push_back(task.taskId()); return true;
}
bool WarehouseWorker::assignPackingTask(const PackingTask& task) {
    if (!onShift_ || task.taskId().empty() ||
        std::find(taskIds_.begin(), taskIds_.end(), task.taskId()) != taskIds_.end()) return false;
    taskIds_.push_back(task.taskId()); return true;
}
bool WarehouseWorker::completeTask(const std::string& taskId) {
    auto it = std::find(taskIds_.begin(), taskIds_.end(), taskId);
    if (it == taskIds_.end()) return false;
    taskIds_.erase(it); return true;
}
double WarehouseWorker::calculateShiftPay(double hoursWorked) const noexcept {
    return hoursWorked < 0.0 ? 0.0 : hoursWorked * hourlyRate_;
}
bool WarehouseWorker::onShift() const noexcept { return onShift_; }
const std::string& WarehouseWorker::employeeId() const noexcept { return employeeId_; }
const std::string& WarehouseWorker::shiftCode() const noexcept { return shiftCode_; }
