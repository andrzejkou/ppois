#pragma once
#include <string>
#include <ctime>
class StockLot { public: StockLot(std::string lotNumber,std::string sku,int quantity,std::time_t expiryDate,double unitCost); bool isExpired(std::time_t at=std::time(nullptr))const noexcept; int remainingShelfLifeDays(std::time_t at=std::time(nullptr))const noexcept; void remove(int); void markQuarantined()noexcept; void releaseFromQuarantine()noexcept; bool quarantined()const noexcept; int quantity()const noexcept; double totalCost()const noexcept; const std::string& lotNumber()const noexcept; const std::string& sku()const noexcept; private: std::string lotNumber_,sku_; int quantity_; std::time_t expiryDate_; double unitCost_; bool quarantined_{false}; };
