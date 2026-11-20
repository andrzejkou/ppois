#include "storage/Warehouse.h"
#include "storage/StorageZone.h"
#include "exceptions/StorageCapacityExceededException.h"
#include "exceptions/WarehouseException.h"
#include <algorithm>
#include <utility>
Warehouse::Warehouse(std::string c,std::string a,double cap):code_(std::move(c)),address_(std::move(a)),capacity_(cap){if(code_.empty()||address_.empty()||cap<=0)throw WarehouseException("Invalid warehouse");}
void Warehouse::addZone(const StorageZone& z){if(z.code().empty())throw WarehouseException("Invalid zone");if(!containsZone(z.code()))zoneCodes_.push_back(z.code());}
bool Warehouse::containsZone(const std::string& c)const{return std::find(zoneCodes_.begin(),zoneCodes_.end(),c)!=zoneCodes_.end();}
void Warehouse::receive(int v){if(!active_||v<=0)throw WarehouseException("Warehouse cannot receive volume");if(v>freeCapacity())throw StorageCapacityExceededException("Warehouse capacity exceeded");occupied_+=v;}
void Warehouse::dispatch(int v){if(v<=0||v>occupied_)throw WarehouseException("Invalid dispatch volume");occupied_-=v;}
double Warehouse::freeCapacity()const noexcept{return capacity_-occupied_;} double Warehouse::occupancyRate()const noexcept{return occupied_/capacity_;} void Warehouse::close()noexcept{active_=false;} void Warehouse::reopen()noexcept{active_=true;}
const std::string& Warehouse::code()const noexcept{return code_;} const std::string& Warehouse::address()const noexcept{return address_;} double Warehouse::capacity()const noexcept{return capacity_;} double Warehouse::occupied()const noexcept{return occupied_;} bool Warehouse::active()const noexcept{return active_;} const std::vector<std::string>& Warehouse::zoneCodes()const noexcept{return zoneCodes_;}
