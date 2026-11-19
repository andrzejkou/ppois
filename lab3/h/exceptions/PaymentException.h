#pragma once
#include "WarehouseException.h"
#include <string>

class PaymentException : public WarehouseException {
public:
    explicit PaymentException(const std::string& message);
};
