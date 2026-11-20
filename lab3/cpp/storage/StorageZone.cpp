#include "storage/StorageZone.h"
#include "exceptions/StorageCapacityExceededException.h"
#include "exceptions/WarehouseException.h"
#include <utility>
StorageZone::StorageZone(std::string c,std::string t,double cap,double occ):code_(std::move(c)),temperatureMode_(std::move(t)),capacity_(cap),occupied_(occ){if(code_.empty()||temperatureMode_.empty()||cap<=0||occ<0||occ>cap)throw WarehouseException("Invalid storage zone");}
bool StorageZone::isSuitableFor(const std::string& mode)const{return mode==temperatureMode_;}
void StorageZone::store(double v){if(v<=0)throw WarehouseException("Stored volume must be positive");if(v>freeCapacity())throw StorageCapacityExceededException("Zone capacity exceeded");occupied_+=v;}
void StorageZone::remove(double v){if(v<=0||v>occupied_)throw WarehouseException("Invalid zone removal");occupied_-=v;}
double StorageZone::freeCapacity()const noexcept{return capacity_-occupied_;} double StorageZone::occupancyRate()const noexcept{return occupied_/capacity_;} const std::string& StorageZone::code()const noexcept{return code_;} const std::string& StorageZone::temperatureMode()const noexcept{return temperatureMode_;} double StorageZone::capacity()const noexcept{return capacity_;} double StorageZone::occupied()const noexcept{return occupied_;}
