#include "inventory/InventoryLine.h"
#include "exceptions/WarehouseException.h"
#include "exceptions/InventoryMismatchException.h"
#include <utility>
InventoryLine::InventoryLine(std::string s,int e):sku_(std::move(s)),expected_(e){if(sku_.empty()||e<0)throw WarehouseException("Invalid inventory line");}
void InventoryLine::recordCount(int q){if(q<0)throw WarehouseException("Count cannot be negative");counted_=q;countedFlag_=true;} int InventoryLine::difference()const{if(!countedFlag_)throw InventoryMismatchException("Inventory line has not been counted");return counted_-expected_;} bool InventoryLine::counted()const noexcept{return countedFlag_;} bool InventoryLine::matches()const{if(!countedFlag_)return false;return counted_==expected_;} const std::string& InventoryLine::sku()const noexcept{return sku_;} int InventoryLine::expectedQuantity()const noexcept{return expected_;} int InventoryLine::countedQuantity()const noexcept{return counted_;}
