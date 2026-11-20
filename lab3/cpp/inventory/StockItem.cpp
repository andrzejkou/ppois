#include "inventory/StockItem.h"
#include "products/Product.h"
#include "exceptions/InsufficientStockException.h"
#include "exceptions/WarehouseException.h"
#include <utility>
StockItem::StockItem(std::string id,std::string sku,std::string wh,std::string bin,int q,int r):id_(std::move(id)),sku_(std::move(sku)),warehouseCode_(std::move(wh)),binCode_(std::move(bin)),onHand_(q),reserved_(r){if(id_.empty()||sku_.empty()||warehouseCode_.empty()||binCode_.empty()||q<0||r<0||r>q)throw WarehouseException("Invalid stock item");}
int StockItem::available()const noexcept{return onHand_-reserved_;} void StockItem::receive(int q){if(q<=0)throw WarehouseException("Receipt quantity must be positive");onHand_+=q;}
void StockItem::reserve(int q){if(q<=0||q>available())throw InsufficientStockException("Not enough available stock to reserve");reserved_+=q;}
void StockItem::release(int q){if(q<=0||q>reserved_)throw WarehouseException("Invalid reservation release");reserved_-=q;}
void StockItem::issue(int q){if(q<=0||q>available())throw InsufficientStockException("Not enough available stock to issue");onHand_-=q;}
void StockItem::moveTo(std::string b){if(b.empty())throw WarehouseException("Destination bin is empty");binCode_=std::move(b);}
const std::string& StockItem::id()const noexcept{return id_;} const std::string& StockItem::sku()const noexcept{return sku_;} const std::string& StockItem::warehouseCode()const noexcept{return warehouseCode_;} const std::string& StockItem::binCode()const noexcept{return binCode_;} int StockItem::onHand()const noexcept{return onHand_;} int StockItem::reserved()const noexcept{return reserved_;}

bool StockItem::matchesProduct(const Product& product) const { return product.active() && product.sku() == sku_; }
