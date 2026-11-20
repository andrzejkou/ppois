#pragma once
#include "inventory/InventoryLine.h"
#include <string>
#include <vector>
enum class InventoryStatus { Planned, InProgress, Completed, Cancelled };
class Warehouse;
class Inventory { public: Inventory(std::string id,std::string warehouseCode); void addLine(const InventoryLine&); void start(); void recordCount(const std::string& sku,int countedQuantity); bool allLinesCounted()const noexcept; int discrepancyCount()const noexcept; int totalDifference()const; void complete(); void cancel(); InventoryStatus status()const noexcept; const std::vector<InventoryLine>& lines()const noexcept; const std::string& id()const noexcept; const std::string& warehouseCode()const noexcept;     bool belongsTo(const Warehouse& warehouse) const;
private: std::string id_,warehouseCode_; std::vector<InventoryLine> lines_; InventoryStatus status_{InventoryStatus::Planned}; };
