#pragma once
#include <stdexcept>
#include <string>

class WarehouseException : public std::runtime_error {
public:
    explicit WarehouseException(const std::string& message);
};
