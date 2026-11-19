#include "AuditLog.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

AuditLog::AuditLog(std::string eventId, std::string actorId, int quantity, double amount)
    : eventId_(std::move(eventId)), actorId_(std::move(actorId)),
      quantity_(quantity), amount_(amount) {
    if (eventId_.empty() || actorId_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating AuditLog");
}

bool AuditLog::recordEvent() const {
    return active_ && !eventId_.empty();
}

bool AuditLog::countEventsByActor(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& AuditLog::eventId() const noexcept { return eventId_; }
const std::string& AuditLog::actorId() const noexcept { return actorId_; }
int AuditLog::quantity() const noexcept { return quantity_; }
double AuditLog::amount() const noexcept { return amount_; }
bool AuditLog::active() const noexcept { return active_; }

void AuditLog::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool AuditLog::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t AuditLog::relatedCount() const noexcept { return relatedIds_.size(); }
