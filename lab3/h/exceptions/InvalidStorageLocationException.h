#pragma once
#include "WarehouseException.h"
#include <string>

class InvalidStorageLocationException : public WarehouseException {
public:
    explicit InvalidStorageLocationException(const std::string& message);
};
