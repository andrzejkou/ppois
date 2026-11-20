#pragma once
#include "WarehouseException.h"
#include <string>

class PurchaseOrderException : public WarehouseException {
public:
    explicit PurchaseOrderException(const std::string& message);
};
