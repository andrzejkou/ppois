#pragma once
#include "WarehouseException.h"
#include <string>

class InventoryMismatchException : public WarehouseException {
public:
    explicit InventoryMismatchException(const std::string& message);
};
