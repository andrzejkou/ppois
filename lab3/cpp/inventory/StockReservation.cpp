#include "inventory/StockReservation.h"
#include "inventory/StockItem.h"
#include "exceptions/ReservationException.h"
#include "exceptions/WarehouseException.h"
#include <utility>
StockReservation::StockReservation(std::string id,std::string order,std::string sku,int q):id_(std::move(id)),orderId_(std::move(order)),sku_(std::move(sku)),quantity_(q){if(id_.empty()||orderId_.empty()||sku_.empty()||q<=0)throw WarehouseException("Invalid reservation");}
void StockReservation::apply(StockItem& item){if(status_!=ReservationStatus::Active||item.sku()!=sku_)throw ReservationException("Reservation cannot be applied to this stock item");item.reserve(quantity_);}
void StockReservation::release(StockItem& item){if(status_!=ReservationStatus::Active||item.sku()!=sku_)throw ReservationException("Reservation cannot be released");item.release(quantity_);status_=ReservationStatus::Released;}
void StockReservation::consume(StockItem& item){if(status_!=ReservationStatus::Active||item.sku()!=sku_)throw ReservationException("Reservation cannot be consumed");item.issue(quantity_);item.release(quantity_);status_=ReservationStatus::Consumed;}
bool StockReservation::active()const noexcept{return status_==ReservationStatus::Active;} int StockReservation::quantity()const noexcept{return quantity_;} ReservationStatus StockReservation::status()const noexcept{return status_;} const std::string& StockReservation::id()const noexcept{return id_;} const std::string& StockReservation::orderId()const noexcept{return orderId_;} const std::string& StockReservation::sku()const noexcept{return sku_;}
