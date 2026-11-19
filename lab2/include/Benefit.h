#pragma once

#include "Premy.h"

#include <string>
#include <vector>

class Benefit : public Premy {
public:
  Benefit(std::string name, double amount, std::string description);

  void assignTo(const std::string &studentId);
  bool assignedTo(const std::string &studentId) const;

private:
  std::string description_;
  std::vector<std::string> studentIds_;
};
