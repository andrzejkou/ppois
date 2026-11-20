#include "procurement/PurchaseOrder.h"
#include "procurement/Supplier.h"
#include "exceptions/PurchaseOrderException.h"
#include "exceptions/InvalidOrderStateException.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
PurchaseOrder::PurchaseOrder(std::string i,std::string s):id_(std::move(i)),supplierId_(std::move(s)){if(id_.empty()||supplierId_.empty())throw WarehouseException("Purchase order identifiers are required");}
void PurchaseOrder::addLine(const PurchaseOrderLine& l){if(status_!=PurchaseOrderStatus::Draft)throw InvalidOrderStateException("Only draft purchase orders can be edited");for(const auto& x:lines_)if(x.sku()==l.sku())throw PurchaseOrderException("Duplicate SKU in purchase order");lines_.push_back(l);}
void PurchaseOrder::approve(){if(status_!=PurchaseOrderStatus::Draft||lines_.empty())throw InvalidOrderStateException("Purchase order cannot be approved");status_=PurchaseOrderStatus::Approved;}
void PurchaseOrder::registerReceipt(const std::string& sku,int q){if(status_!=PurchaseOrderStatus::Approved&&status_!=PurchaseOrderStatus::PartiallyReceived)throw InvalidOrderStateException("Purchase order is not receivable");auto it=std::find_if(lines_.begin(),lines_.end(),[&](const auto& l){return l.sku()==sku;});if(it==lines_.end())throw PurchaseOrderException("SKU is not present in purchase order");it->receive(q);status_=fullyReceived()?PurchaseOrderStatus::Received:PurchaseOrderStatus::PartiallyReceived;}
bool PurchaseOrder::fullyReceived()const noexcept{if(lines_.empty())return false;for(const auto& l:lines_)if(l.outstanding()!=0)return false;return true;} double PurchaseOrder::totalCost()const noexcept{double t=0;for(const auto& l:lines_)t+=l.lineTotal();return t;} void PurchaseOrder::cancel(){if(status_==PurchaseOrderStatus::Received||status_==PurchaseOrderStatus::Cancelled)throw InvalidOrderStateException("Purchase order cannot be cancelled");status_=PurchaseOrderStatus::Cancelled;} PurchaseOrderStatus PurchaseOrder::status()const noexcept{return status_;} const std::vector<PurchaseOrderLine>& PurchaseOrder::lines()const noexcept{return lines_;} const std::string& PurchaseOrder::id()const noexcept{return id_;} const std::string& PurchaseOrder::supplierId()const noexcept{return supplierId_;}

bool PurchaseOrder::suppliedBy(const Supplier& supplier) const { return supplier.active() && supplier.supplierId() == supplierId_; }
