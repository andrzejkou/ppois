#pragma once
#include <string>
#include <vector>
class Barcode;
class Product {
public:
 Product(std::string sku,std::string name,double unitPrice,int reorderLevel=0,std::string categoryCode={},std::string brandCode={});
 void changePrice(double); bool isAvailableForSale(int requested,int availableStock) const; bool needsReorder(int availableStock) const noexcept;
 void deactivate() noexcept; void activate() noexcept; void addBarcode(const std::string&); bool hasBarcode(const std::string&) const;
 const std::string& sku() const noexcept; const std::string& name() const noexcept; double unitPrice() const noexcept; int reorderLevel() const noexcept;
 const std::string& categoryCode() const noexcept; const std::string& brandCode() const noexcept; bool active() const noexcept; const std::vector<std::string>& barcodes() const noexcept;
    bool registerBarcode(const Barcode& barcode);
private: std::string sku_,name_; double unitPrice_; int reorderLevel_; std::string categoryCode_,brandCode_; bool active_{true}; std::vector<std::string> barcodes_;
};
