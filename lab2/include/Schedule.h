#pragma once

#include "Week.h"

#include <cstddef>
#include <string>
#include <vector>

class Schedule {
public:
  explicit Schedule(std::string groupCode);
  void addWeek(const Week &weekEx);
  void changeSchedule();
  std::size_t lessonCount() const;
  const std::string &groupCode() const;

private:
  std::string groupCode_;
  std::vector<Week> weeks_;
};
