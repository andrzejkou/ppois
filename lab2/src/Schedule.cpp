
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
