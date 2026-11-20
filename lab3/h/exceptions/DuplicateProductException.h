#pragma once
#include "WarehouseException.h"
#include <string>

class DuplicateProductException : public WarehouseException {
public:
    explicit DuplicateProductException(const std::string& message);
};
