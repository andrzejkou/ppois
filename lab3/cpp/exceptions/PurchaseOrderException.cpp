#include "PurchaseOrderException.h"

PurchaseOrderException::PurchaseOrderException(const std::string& message)
    : WarehouseException(message) {}
