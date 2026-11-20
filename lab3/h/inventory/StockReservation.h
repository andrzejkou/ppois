#pragma once
#include <string>
class StockItem;
enum class ReservationStatus { Active, Released, Consumed };
class StockReservation {
public: StockReservation(std::string id,std::string orderId,std::string sku,int quantity);
 void apply(StockItem&); void release(StockItem&); void consume(StockItem&); bool active()const noexcept; int quantity()const noexcept; ReservationStatus status()const noexcept; const std::string& id()const noexcept; const std::string& orderId()const noexcept; const std::string& sku()const noexcept;
private: std::string id_,orderId_,sku_; int quantity_; ReservationStatus status_{ReservationStatus::Active};
};
