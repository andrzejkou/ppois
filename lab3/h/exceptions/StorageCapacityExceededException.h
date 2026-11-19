#pragma once
#include "WarehouseException.h"
#include <string>

class StorageCapacityExceededException : public WarehouseException {
public:
    explicit StorageCapacityExceededException(const std::string& message);
};
