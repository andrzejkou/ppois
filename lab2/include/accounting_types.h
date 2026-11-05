#pragma once

#include <string>
#include <vector>

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

class Scholarship : public Premy {
public:
  Scholarship(std::string name, double amount);
  void changeAmount(double amount) override;
};

class Benefit : public Premy {
public:
  Benefit(std::string name, double amount, std::string description);

  void assignTo(const std::string &studentId);
  bool assignedTo(const std::string &studentId) const;

private:
  std::string description_;
  std::vector<std::string> studentIds_;
};
