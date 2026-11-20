#pragma once
#include <string>
class SalesOrderLine { public: SalesOrderLine(std::string sku,int quantity,double unitPrice); double lineTotal()const noexcept; void changeQuantity(int); void applyDiscount(double); const std::string& sku()const noexcept; int quantity()const noexcept; double unitPrice()const noexcept; double discount()const noexcept; private: std::string sku_; int quantity_; double unitPrice_; double discount_{0}; };
