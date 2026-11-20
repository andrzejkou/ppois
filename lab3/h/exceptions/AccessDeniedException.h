#pragma once
#include "WarehouseException.h"
#include <string>

class AccessDeniedException : public WarehouseException {
public:
    explicit AccessDeniedException(const std::string& message);
};
