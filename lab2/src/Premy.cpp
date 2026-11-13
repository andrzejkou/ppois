#include "accounting_types.h"

#include "Premy.h"

#include <stdexcept>
#include <utility>

Premy::Premy(std::string name, double amount)
    : name_(std::move(name)), amount_(amount) {

  if (name_.empty()) {
    throw std::invalid_argument("Premy name cannot be empty");
  }

  if (amount_ < 0) {
    throw std::invalid_argument("Premy amount cannot be negative");
  }
}

void Premy::changeAmount(double amount) {
  if (amount < 0) {
    throw std::invalid_argument("Premy amount cannot be negative");
  }

  amount_ = amount;
}

const std::string &Premy::name() const { return name_; }

double Premy::amount() const { return amount_; }
