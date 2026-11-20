#pragma once
#include "procurement/PurchaseOrderLine.h"
#include <string>
#include <vector>
enum class PurchaseOrderStatus { Draft, Approved, PartiallyReceived, Received, Cancelled };
class PurchaseOrder { public: PurchaseOrder(std::string id,std::string supplierId); void addLine(const PurchaseOrderLine&); void approve(); void registerReceipt(const std::string& sku,int quantity); bool fullyReceived()const noexcept; double totalCost()const noexcept; void cancel(); PurchaseOrderStatus status()const noexcept; const std::vector<PurchaseOrderLine>& lines()const noexcept; const std::string& id()const noexcept; const std::string& supplierId()const noexcept; private: std::string id_,supplierId_; std::vector<PurchaseOrderLine> lines_; PurchaseOrderStatus status_{PurchaseOrderStatus::Draft}; };
