#pragma once
#include <string>
#include <vector>
class StorageZone;
class Warehouse {
public: Warehouse(std::string code,std::string address,double capacity); void addZone(const StorageZone&); bool containsZone(const std::string&)const;
 void receive(int volume); void dispatch(int volume); double freeCapacity()const noexcept; double occupancyRate()const noexcept; void close()noexcept; void reopen()noexcept;
 const std::string& code()const noexcept; const std::string& address()const noexcept; double capacity()const noexcept; double occupied()const noexcept; bool active()const noexcept; const std::vector<std::string>& zoneCodes()const noexcept;
private: std::string code_,address_; double capacity_,occupied_{0}; bool active_{true}; std::vector<std::string> zoneCodes_;
};
