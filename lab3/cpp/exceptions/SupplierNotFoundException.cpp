#include "SupplierNotFoundException.h"

SupplierNotFoundException::SupplierNotFoundException(const std::string& message)
    : WarehouseException(message) {}
