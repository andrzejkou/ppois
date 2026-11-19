#include "Barcode.h"
#include "exceptions/WarehouseException.h"

#include <algorithm>
#include <utility>

Barcode::Barcode(std::string barcodeValue, std::string format, int quantity, double amount)
    : barcodeValue_(std::move(barcodeValue)), format_(std::move(format)),
      quantity_(quantity), amount_(amount) {
    if (barcodeValue_.empty() || format_.empty() || quantity_ < 0 || amount_ < 0.0)
        throw WarehouseException("Invalid data while creating Barcode");
}

bool Barcode::validateBarcode() const {
    return active_ && !barcodeValue_ .empty() && quantity_ >= 0;
}

bool Barcode::matches(int delta) {
    if (!active_ || delta < 0) return false;
    quantity_ += delta;
    return true;
}

const std::string& Barcode::barcodeValue() const noexcept { return barcodeValue_; }
const std::string& Barcode::format() const noexcept { return format_; }
int Barcode::quantity() const noexcept { return quantity_; }
double Barcode::amount() const noexcept { return amount_; }
bool Barcode::active() const noexcept { return active_; }

void Barcode::addRelatedId(const std::string& relatedId) {
    if (relatedId.empty() || hasRelatedId(relatedId)) return;
    relatedIds_.push_back(relatedId);
}

bool Barcode::hasRelatedId(const std::string& relatedId) const {
    return std::find(relatedIds_.begin(), relatedIds_.end(), relatedId) != relatedIds_.end();
}

std::size_t Barcode::relatedCount() const noexcept { return relatedIds_.size(); }
