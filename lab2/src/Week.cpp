
#include "Week.h"

#include <algorithm>
#include <stdexcept>

Week::Week(int number) : number_(number) {
  if (number_ <= 0) {
    throw std::invalid_argument("Week number must be positive");
  }
}

void Week::addDay(const Day &day) {
  if (days_.size() >= 7) {
    throw std::length_error("A week cannot contain more than 7 days");
  }

  const auto it =
      std::find_if(days_.begin(), days_.end(), [&day](const Day &existingDay) {
        return existingDay.getDate() == day.getDate();
      });

  if (it != days_.end()) {
    throw std::invalid_argument("This day is already added to the week");
  }

  days_.push_back(day);
}
std::size_t Week::lessonCount() const {
  std::size_t count = 0;

  for (const Day &day : days_) {
    count += day.lessonCount();
  }

  return count;
}
int Week::number() const { return number_; }
