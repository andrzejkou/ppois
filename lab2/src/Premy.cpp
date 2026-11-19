#include "accounting_types.h"

#include "Premy.h"

#include <cmath>
#include <stdexcept>
#include <utility>

Premy::Premy(std::string name, double amount)
    : name_(std::move(name)), amount_(amount) {

  if (name_.empty()) {
    throw std::invalid_argument("Premy name cannot be empty");
  }

  if (!std::isfinite(amount_) || amount_ < 0) {
    throw std::invalid_argument("Premy amount must be finite and non-negative");
  }
}

void Premy::changeAmount(double amount) {
  if (!std::isfinite(amount) || amount < 0) {
    throw std::invalid_argument("Premy amount must be finite and non-negative");
  }

  amount_ = amount;
}

const std::string &Premy::name() const { return name_; }

double Premy::amount() const { return amount_; }
