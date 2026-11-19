#pragma once

#include <string>

class Classroom {
public:
  Classroom(std::string roomNumber, int capacity);
  const std::string &roomNumber() const;
  int capacity() const;
  bool canFit(int peopleCount) const;

private:
  std::string roomNumber_;
  int capacity_;
};
