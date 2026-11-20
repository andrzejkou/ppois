#include "InsufficientStockException.h"

InsufficientStockException::InsufficientStockException(const std::string& message)
    : WarehouseException(message) {}
