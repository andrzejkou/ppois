#pragma once
#include "WarehouseException.h"
#include <string>

class ProductNotFoundException : public WarehouseException {
public:
    explicit ProductNotFoundException(const std::string& message);
};
