#include "InvalidStorageLocationException.h"

InvalidStorageLocationException::InvalidStorageLocationException(const std::string& message)
    : WarehouseException(message) {}
