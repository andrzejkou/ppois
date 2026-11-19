#include "PickingTask.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

PickingTask::PickingTask(std::string taskId, std::string orderId, int quantity, double amount)
    : taskId_(std::move(taskId)), orderId_(std::move(orderId)),
      quantity_(quantity), amount_(amount) {
    if (taskId_.empty() || orderId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating PickingTask");
}

bool PickingTask::assignPicker() const {
    return active_ && !taskId_.empty();
}

bool PickingTask::markPicked(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& PickingTask::taskId() const noexcept { return taskId_; }
const std::string& PickingTask::orderId() const noexcept { return orderId_; }
int PickingTask::quantity() const noexcept { return quantity_; }
double PickingTask::amount() const noexcept { return amount_; }
bool PickingTask::active() const noexcept { return active_; }

void PickingTask::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool PickingTask::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t PickingTask::relatedCount() const noexcept { return relatedIds_.size(); }
