#pragma once

#include <string>

class Dean;

class AcademicOrder {
public:
  AcademicOrder(int number, std::string title);
  void approve(const Dean &dean);
  bool isApproved() const;
  int number() const;
  const std::string &title() const;

private:
  int number_;
  std::string title_;
  bool approved_{false};
};
