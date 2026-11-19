#include "ReservationException.h"

ReservationException::ReservationException(const std::string& message)
    : WarehouseException(message) {}
