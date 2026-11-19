#pragma once

#include <string>

class Subject {
public:
  Subject(std::string code, std::string name, int credits);
  const std::string &code() const;
  const std::string &name() const;
  int credits() const;

private:
  std::string code_;
  std::string name_;
  int credits_;
};
