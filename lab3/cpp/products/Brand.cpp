#include "Brand.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Brand::Brand(std::string brandCode, std::string name, int quantity, double amount)
    : brandCode_(std::move(brandCode)), name_(std::move(name)),
      quantity_(quantity), amount_(amount) {
    if (brandCode_.empty() || name_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Brand");
}

bool Brand::renameBrand() const {
    return active_ && !brandCode_.empty();
}

bool Brand::matchesProduct(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Brand::brandCode() const noexcept { return brandCode_; }
const std::string& Brand::name() const noexcept { return name_; }
int Brand::quantity() const noexcept { return quantity_; }
double Brand::amount() const noexcept { return amount_; }
bool Brand::active() const noexcept { return active_; }

void Brand::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Brand::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Brand::relatedCount() const noexcept { return relatedIds_.size(); }
