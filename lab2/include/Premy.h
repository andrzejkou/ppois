#pragma once

#include <string>

class Premy {

public:
  Premy(std::string name, double amount);
  virtual ~Premy() = default;

  virtual void changeAmount(double amount);
  const std::string &name() const;
  double amount() const;

private:
  std::string name_;
  double amount_;
};
