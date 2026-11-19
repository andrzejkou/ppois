#include "WarehouseException.h"

WarehouseException::WarehouseException(const std::string& message)
    : std::runtime_error(message) {}
