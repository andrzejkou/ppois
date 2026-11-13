#pragma once

#include "Day.h"

#include <vector>

class Week {
public:
  Week(int number);
  void addDay(const Day &day);
  int number() const;
  std::size_t lessonCount() const;

private:
  int number_;
  std::vector<Day> days_;
};
