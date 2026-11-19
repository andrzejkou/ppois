#include "ExpiredProductException.h"

ExpiredProductException::ExpiredProductException(const std::string& message)
    : WarehouseException(message) {}
