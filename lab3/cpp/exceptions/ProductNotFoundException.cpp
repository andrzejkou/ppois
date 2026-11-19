#include "ProductNotFoundException.h"

ProductNotFoundException::ProductNotFoundException(const std::string& message)
    : WarehouseException(message) {}
