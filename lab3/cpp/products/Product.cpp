#include "products/Product.h"
#include "exceptions/DuplicateProductException.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
Product::Product(std::string s,std::string n,double p,int r,std::string c,std::string b):sku_(std::move(s)),name_(std::move(n)),unitPrice_(p),reorderLevel_(r),categoryCode_(std::move(c)),brandCode_(std::move(b)){if(sku_.empty()||name_.empty()||p<0||r<0)throw WarehouseException("Invalid product data");}
void Product::changePrice(double p){if(p<0)throw WarehouseException("Negative product price");unitPrice_=p;}
bool Product::isAvailableForSale(int requested,int stock)const{return active_&&requested>0&&stock>=requested;}
bool Product::needsReorder(int stock)const noexcept{return active_&&stock<=reorderLevel_;}
void Product::deactivate()noexcept{active_=false;} void Product::activate()noexcept{active_=true;}
void Product::addBarcode(const std::string& b){if(b.empty())throw WarehouseException("Empty barcode");if(hasBarcode(b))throw DuplicateProductException("Duplicate barcode");barcodes_.push_back(b);}
bool Product::hasBarcode(const std::string& b)const{return std::find(barcodes_.begin(),barcodes_.end(),b)!=barcodes_.end();}
const std::string& Product::sku()const noexcept{return sku_;} const std::string& Product::name()const noexcept{return name_;} double Product::unitPrice()const noexcept{return unitPrice_;} int Product::reorderLevel()const noexcept{return reorderLevel_;} const std::string& Product::categoryCode()const noexcept{return categoryCode_;} const std::string& Product::brandCode()const noexcept{return brandCode_;} bool Product::active()const noexcept{return active_;} const std::vector<std::string>& Product::barcodes()const noexcept{return barcodes_;}
