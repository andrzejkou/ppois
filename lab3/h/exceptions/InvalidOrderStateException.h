#pragma once
#include "WarehouseException.h"
#include <string>

class InvalidOrderStateException : public WarehouseException {
public:
    explicit InvalidOrderStateException(const std::string& message);
};
