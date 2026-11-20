#pragma once
#include <string>
class InventoryLine { public: InventoryLine(std::string sku,int expectedQuantity); void recordCount(int countedQuantity); int difference()const; bool counted()const noexcept; bool matches()const; const std::string& sku()const noexcept; int expectedQuantity()const noexcept; int countedQuantity()const noexcept; private: std::string sku_; int expected_,counted_{0}; bool countedFlag_{false}; };
