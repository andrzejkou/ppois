#pragma once
#include "WarehouseException.h"
#include <string>

class InsufficientStockException : public WarehouseException {
public:
    explicit InsufficientStockException(const std::string& message);
};
