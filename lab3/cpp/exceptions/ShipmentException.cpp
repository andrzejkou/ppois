#include "ShipmentException.h"

ShipmentException::ShipmentException(const std::string& message)
    : WarehouseException(message) {}
