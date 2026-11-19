#pragma once

#include "Day.h"

#include <vector>

class Week {
public:
  Week(int number);
  void addDay(const Day &day);
  int number() const;
  std::size_t lessonCount() const;
  bool removeDay(const std::string &date);
  bool hasDay(const std::string &date) const;
  std::size_t dayCount() const;
  bool hasTeacher(const std::string &teacherId) const;
  bool hasTimeConflict(const std::string &dayName, int startHour,
                       int durationHours) const;

private:
  int number_;
  std::vector<Day> days_;
};
