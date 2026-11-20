#pragma once
#include <string>
#include <vector>
class StorageZone { public: StorageZone(std::string code,std::string temperatureMode,double capacity,double occupied=0); bool isSuitableFor(const std::string& productMode)const; void store(double volume); void remove(double volume); double freeCapacity()const noexcept; double occupancyRate()const noexcept; const std::string& code()const noexcept; const std::string& temperatureMode()const noexcept; double capacity()const noexcept; double occupied()const noexcept; private: std::string code_,temperatureMode_; double capacity_,occupied_; };
