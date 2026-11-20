#include "inventory/StockLot.h"
#include "products/Product.h"
#include "exceptions/ExpiredProductException.h"
#include "exceptions/InsufficientStockException.h"
#include "exceptions/WarehouseException.h"
#include <utility>
StockLot::StockLot(std::string l,std::string s,int q,std::time_t e,double c):lotNumber_(std::move(l)),sku_(std::move(s)),quantity_(q),expiryDate_(e),unitCost_(c){if(lotNumber_.empty()||sku_.empty()||q<0||e<=0||c<0)throw WarehouseException("Invalid stock lot");}
bool StockLot::isExpired(std::time_t at)const noexcept{return at>=expiryDate_;} int StockLot::remainingShelfLifeDays(std::time_t at)const noexcept{return static_cast<int>((expiryDate_-at)/86400);}
void StockLot::remove(int q){if(isExpired())throw ExpiredProductException("Cannot issue expired lot");if(quarantined_)throw WarehouseException("Lot is quarantined");if(q<=0||q>quantity_)throw InsufficientStockException("Insufficient lot quantity");quantity_-=q;}
void StockLot::markQuarantined()noexcept{quarantined_=true;} void StockLot::releaseFromQuarantine()noexcept{quarantined_=false;} bool StockLot::quarantined()const noexcept{return quarantined_;} int StockLot::quantity()const noexcept{return quantity_;} double StockLot::totalCost()const noexcept{return quantity_*unitCost_;} const std::string& StockLot::lotNumber()const noexcept{return lotNumber_;} const std::string& StockLot::sku()const noexcept{return sku_;}

bool StockLot::matchesProduct(const Product& product) const { return product.active() && product.sku() == sku_; }
