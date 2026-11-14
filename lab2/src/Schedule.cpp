
#include "Schedule.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

Schedule::Schedule(std::string groupCode) : groupCode_(std::move(groupCode)) {
  if (groupCode_.empty()) {
    throw std::invalid_argument("Group code cannot be empty");
  }
}

void Schedule::addWeek(const Week &weekEx) {
  const auto it =
      std::find_if(weeks_.begin(), weeks_.end(), [&weekEx](const Week &week) {
        return week.number() == weekEx.number();
      });

  if (it != weeks_.end()) {
    throw std::invalid_argument("This week is already in the schedule");
  }

  weeks_.push_back(weekEx);
}

void Schedule::changeSchedule() {
  std::sort(weeks_.begin(), weeks_.end(),
            [](const Week &first, const Week &second) {
              return first.number() < second.number();
            });
}

std::size_t Schedule::lessonCount() const {
  std::size_t count = 0;

  for (const Week &week : weeks_) {
    count += week.lessonCount();
  }

  return count;
}

const std::string &Schedule::groupCode() const { return groupCode_; }

bool Schedule::removeWeek(int weekNumber) {
  const auto it = std::find_if(
      weeks_.begin(), weeks_.end(), [weekNumber](const Week &week) {
        return week.number() == weekNumber;
      });
  if (it == weeks_.end()) {
    return false;
  }
  weeks_.erase(it);
  return true;
}

bool Schedule::hasWeek(int weekNumber) const {
  return std::any_of(weeks_.begin(), weeks_.end(),
                     [weekNumber](const Week &week) {
                       return week.number() == weekNumber;
                     });
}

std::size_t Schedule::weekCount() const { return weeks_.size(); }

std::vector<int> Schedule::weekNumbers() const {
  std::vector<int> result;
  result.reserve(weeks_.size());
  for (const Week &week : weeks_) {
    result.push_back(week.number());
  }
  std::sort(result.begin(), result.end());
  return result;
}

bool Schedule::hasTeacher(const std::string &teacherId) const {
  return std::any_of(weeks_.begin(), weeks_.end(),
                     [&teacherId](const Week &week) {
                       return week.hasTeacher(teacherId);
                     });
}

bool Schedule::hasTimeConflict(int weekNumber, const std::string &dayName,
                               int startHour, int durationHours) const {
  const auto it = std::find_if(
      weeks_.begin(), weeks_.end(), [weekNumber](const Week &week) {
        return week.number() == weekNumber;
      });
  return it != weeks_.end() &&
         it->hasTimeConflict(dayName, startHour, durationHours);
}
