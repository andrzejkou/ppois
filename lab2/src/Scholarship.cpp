#include "Scholarship.h"

#include <utility>

Scholarship::Scholarship(std::string name, double amount)
    : Premy(std::move(name), amount) {}

void Scholarship::changeAmount(double amount) { Premy::changeAmount(amount); }
