#include "DuplicateProductException.h"

DuplicateProductException::DuplicateProductException(const std::string& message)
    : WarehouseException(message) {}
