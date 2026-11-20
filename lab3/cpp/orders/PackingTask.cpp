#include "PackingTask.h"
#include "orders/SalesOrder.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

PackingTask::PackingTask(std::string taskId, std::string orderId, int quantity, double amount)
    : taskId_(std::move(taskId)), orderId_(std::move(orderId)),
      quantity_(quantity), amount_(amount) {
    if (taskId_.empty() || orderId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating PackingTask");
}

double PackingTask::calculatePackageCount() const {
    return amount_ * static_cast<double>(quantity_);
}

bool PackingTask::completePacking(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& PackingTask::taskId() const noexcept { return taskId_; }
const std::string& PackingTask::orderId() const noexcept { return orderId_; }
int PackingTask::quantity() const noexcept { return quantity_; }
double PackingTask::amount() const noexcept { return amount_; }
bool PackingTask::active() const noexcept { return active_; }

void PackingTask::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool PackingTask::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t PackingTask::relatedCount() const noexcept { return relatedIds_.size(); }

bool PackingTask::belongsTo(const SalesOrder& order) const { return active_ && order.orderId() == orderId_; }

bool PackingTask::requiresOversizePackaging(double threshold) const noexcept { return threshold >= 0.0 && amount_ > threshold; }
