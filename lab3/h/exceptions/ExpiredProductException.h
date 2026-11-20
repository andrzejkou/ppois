#pragma once
#include "WarehouseException.h"
#include <string>

class ExpiredProductException : public WarehouseException {
public:
    explicit ExpiredProductException(const std::string& message);
};
