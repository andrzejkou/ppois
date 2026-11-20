#include "orders/SalesOrderLine.h"
#include "exceptions/WarehouseException.h"
#include <utility>
SalesOrderLine::SalesOrderLine(std::string s,int q,double p):sku_(std::move(s)),quantity_(q),unitPrice_(p){if(sku_.empty()||q<=0||p<0)throw WarehouseException("Invalid sales order line");}
double SalesOrderLine::lineTotal()const noexcept{return quantity_*unitPrice_*(1.0-discount_/100.0);} void SalesOrderLine::changeQuantity(int q){if(q<=0)throw WarehouseException("Quantity must be positive");quantity_=q;} void SalesOrderLine::applyDiscount(double d){if(d<0||d>100)throw WarehouseException("Discount must be between 0 and 100");discount_=d;} const std::string& SalesOrderLine::sku()const noexcept{return sku_;} int SalesOrderLine::quantity()const noexcept{return quantity_;} double SalesOrderLine::unitPrice()const noexcept{return unitPrice_;} double SalesOrderLine::discount()const noexcept{return discount_;}
