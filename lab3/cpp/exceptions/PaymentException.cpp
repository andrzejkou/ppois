#include "PaymentException.h"

PaymentException::PaymentException(const std::string& message)
    : WarehouseException(message) {}
