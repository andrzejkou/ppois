#include "InvalidOrderStateException.h"

InvalidOrderStateException::InvalidOrderStateException(const std::string& message)
    : WarehouseException(message) {}
