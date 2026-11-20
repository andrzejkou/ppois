#pragma once
#include <string>
class Product;
class StockItem {
public: StockItem(std::string id,std::string sku,std::string warehouseCode,std::string binCode,int onHand=0,int reserved=0);
 int available()const noexcept; void receive(int); void reserve(int); void release(int); void issue(int); void moveTo(std::string binCode);
 const std::string& id()const noexcept; const std::string& sku()const noexcept; const std::string& warehouseCode()const noexcept; const std::string& binCode()const noexcept; int onHand()const noexcept; int reserved()const noexcept;
    bool matchesProduct(const Product& product) const;
private: std::string id_,sku_,warehouseCode_,binCode_; int onHand_,reserved_;
};
