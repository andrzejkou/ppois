#include "InventoryMismatchException.h"

InventoryMismatchException::InventoryMismatchException(const std::string& message)
    : WarehouseException(message) {}
