#pragma once
#include "orders/SalesOrderLine.h"
#include <string>
#include <vector>
enum class SalesOrderStatus { Draft, Confirmed, Picking, Shipped, Cancelled };
class SalesOrder { public: SalesOrder(std::string id,std::string customerId); void addLine(const SalesOrderLine&); void removeLine(const std::string& sku); double calculateTotal()const noexcept; void confirm(); void beginPicking(); void markShipped(); void cancel(); SalesOrderStatus status()const noexcept; const std::string& orderId()const noexcept; const std::string& customerId()const noexcept; const std::vector<SalesOrderLine>& lines()const noexcept; private: std::string orderId_,customerId_; std::vector<SalesOrderLine> lines_; SalesOrderStatus status_{SalesOrderStatus::Draft}; };
