#pragma once
#include "WarehouseException.h"
#include <string>

class ShipmentException : public WarehouseException {
public:
    explicit ShipmentException(const std::string& message);
};
