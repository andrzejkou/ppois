#include "procurement/PurchaseOrderLine.h"
#include "exceptions/PurchaseOrderException.h"
#include "exceptions/WarehouseException.h"
#include <utility>
PurchaseOrderLine::PurchaseOrderLine(std::string s,int q,double c):sku_(std::move(s)),ordered_(q),unitCost_(c){if(sku_.empty()||q<=0||c<0)throw WarehouseException("Invalid purchase order line");}
void PurchaseOrderLine::receive(int q){if(q<=0||q>outstanding())throw PurchaseOrderException("Received quantity exceeds outstanding quantity");received_+=q;} int PurchaseOrderLine::outstanding()const noexcept{return ordered_-received_;} double PurchaseOrderLine::lineTotal()const noexcept{return ordered_*unitCost_;} const std::string& PurchaseOrderLine::sku()const noexcept{return sku_;} int PurchaseOrderLine::ordered()const noexcept{return ordered_;} int PurchaseOrderLine::received()const noexcept{return received_;}
