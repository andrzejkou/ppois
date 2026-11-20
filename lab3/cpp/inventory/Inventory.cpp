#include "inventory/Inventory.h"
#include "storage/Warehouse.h"
#include "exceptions/InvalidOrderStateException.h"
#include "exceptions/InventoryMismatchException.h"
#include "exceptions/WarehouseException.h"
#include <utility>
Inventory::Inventory(std::string i,std::string w):id_(std::move(i)),warehouseCode_(std::move(w)){if(id_.empty()||warehouseCode_.empty())throw WarehouseException("Inventory identifiers are required");}
void Inventory::addLine(const InventoryLine& l){if(status_!=InventoryStatus::Planned)throw InvalidOrderStateException("Cannot edit started inventory");for(const auto& x:lines_)if(x.sku()==l.sku())throw WarehouseException("Duplicate SKU in inventory");lines_.push_back(l);}
void Inventory::start(){if(status_!=InventoryStatus::Planned||lines_.empty())throw InvalidOrderStateException("Inventory cannot be started");status_=InventoryStatus::InProgress;}
void Inventory::recordCount(const std::string& sku,int q){if(status_!=InventoryStatus::InProgress)throw InvalidOrderStateException("Inventory is not in progress");for(auto& line:lines_)if(line.sku()==sku){line.recordCount(q);return;}throw InventoryMismatchException("SKU is not part of this inventory");}
bool Inventory::allLinesCounted()const noexcept{for(const auto& l:lines_)if(!l.counted())return false;return !lines_.empty();} int Inventory::discrepancyCount()const noexcept{int n=0;for(const auto& l:lines_)if(l.counted()&&!l.matches())++n;return n;} int Inventory::totalDifference()const{if(!allLinesCounted())throw InventoryMismatchException("Not all inventory lines are counted");int n=0;for(const auto& l:lines_)n+=l.difference();return n;}
void Inventory::complete(){if(status_!=InventoryStatus::InProgress||!allLinesCounted())throw InventoryMismatchException("Cannot complete inventory with uncounted lines");status_=InventoryStatus::Completed;} void Inventory::cancel(){if(status_==InventoryStatus::Completed||status_==InventoryStatus::Cancelled)throw InvalidOrderStateException("Inventory cannot be cancelled");status_=InventoryStatus::Cancelled;} InventoryStatus Inventory::status()const noexcept{return status_;} const std::vector<InventoryLine>& Inventory::lines()const noexcept{return lines_;} const std::string& Inventory::id()const noexcept{return id_;} const std::string& Inventory::warehouseCode()const noexcept{return warehouseCode_;}

bool Inventory::belongsTo(const Warehouse& warehouse) const { return warehouse.active() && warehouse.code() == warehouseCode_; }
