#pragma once
#include "WarehouseException.h"
#include <string>

class ReservationException : public WarehouseException {
public:
    explicit ReservationException(const std::string& message);
};
