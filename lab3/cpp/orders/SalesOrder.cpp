#include "orders/SalesOrder.h"
#include "exceptions/InvalidOrderStateException.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
SalesOrder::SalesOrder(std::string id,std::string c):orderId_(std::move(id)),customerId_(std::move(c)){if(orderId_.empty()||customerId_.empty())throw WarehouseException("Order and customer identifiers are required");}
void SalesOrder::addLine(const SalesOrderLine& l){if(status_!=SalesOrderStatus::Draft)throw InvalidOrderStateException("Only draft orders can be edited");auto i=std::find_if(lines_.begin(),lines_.end(),[&](const auto& x){return x.sku()==l.sku();});if(i!=lines_.end())throw WarehouseException("Duplicate SKU in order");lines_.push_back(l);}
void SalesOrder::removeLine(const std::string& sku){if(status_!=SalesOrderStatus::Draft)throw InvalidOrderStateException("Only draft orders can be edited");auto i=std::find_if(lines_.begin(),lines_.end(),[&](const auto& x){return x.sku()==sku;});if(i==lines_.end())throw WarehouseException("Order line not found");lines_.erase(i);}
double SalesOrder::calculateTotal()const noexcept{double t=0;for(const auto& l:lines_)t+=l.lineTotal();return t;}
void SalesOrder::confirm(){if(status_!=SalesOrderStatus::Draft||lines_.empty())throw InvalidOrderStateException("Cannot confirm empty or non-draft order");status_=SalesOrderStatus::Confirmed;}
void SalesOrder::beginPicking(){if(status_!=SalesOrderStatus::Confirmed)throw InvalidOrderStateException("Order must be confirmed before picking");status_=SalesOrderStatus::Picking;}
void SalesOrder::markShipped(){if(status_!=SalesOrderStatus::Picking)throw InvalidOrderStateException("Order must be picked before shipment");status_=SalesOrderStatus::Shipped;}
void SalesOrder::cancel(){if(status_==SalesOrderStatus::Shipped||status_==SalesOrderStatus::Cancelled)throw InvalidOrderStateException("Order cannot be cancelled in current state");status_=SalesOrderStatus::Cancelled;}
SalesOrderStatus SalesOrder::status()const noexcept{return status_;} const std::string& SalesOrder::orderId()const noexcept{return orderId_;} const std::string& SalesOrder::customerId()const noexcept{return customerId_;} const std::vector<SalesOrderLine>& SalesOrder::lines()const noexcept{return lines_;}
