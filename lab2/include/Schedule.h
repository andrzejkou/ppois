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
  bool removeWeek(int weekNumber);
  std::size_t removeWeeksWithoutLessons();
  bool hasWeek(int weekNumber) const;
  std::size_t weekCount() const;
  std::vector<int> weekNumbers() const;
  bool hasTeacher(const std::string &teacherId) const;
  bool hasTimeConflict(int weekNumber, const std::string &dayName,
                       int startHour, int durationHours) const;
  const std::string &groupCode() const;

private:
  std::string groupCode_;
  std::vector<Week> weeks_;
};
