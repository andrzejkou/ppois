#pragma once
#include "WarehouseException.h"
#include <string>

class SupplierNotFoundException : public WarehouseException {
public:
    explicit SupplierNotFoundException(const std::string& message);
};
