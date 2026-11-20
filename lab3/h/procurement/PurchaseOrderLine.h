#pragma once
#include <string>
class PurchaseOrderLine { public: PurchaseOrderLine(std::string sku,int ordered,double unitCost); void receive(int quantity); int outstanding()const noexcept; double lineTotal()const noexcept; const std::string& sku()const noexcept; int ordered()const noexcept; int received()const noexcept; private: std::string sku_; int ordered_,received_{0}; double unitCost_; };
