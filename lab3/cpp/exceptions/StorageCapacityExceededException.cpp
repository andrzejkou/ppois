#include "StorageCapacityExceededException.h"

StorageCapacityExceededException::StorageCapacityExceededException(const std::string& message)
    : WarehouseException(message) {}
