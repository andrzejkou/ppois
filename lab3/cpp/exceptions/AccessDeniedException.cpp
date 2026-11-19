#include "AccessDeniedException.h"

AccessDeniedException::AccessDeniedException(const std::string& message)
    : WarehouseException(message) {}
