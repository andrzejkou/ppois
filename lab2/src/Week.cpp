
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

bool Week::removeDay(const std::string &date) {
  const auto it = std::find_if(
      days_.begin(), days_.end(), [&date](const Day &day) {
        return day.getDate() == date;
      });
  if (it == days_.end()) {
    return false;
  }
  days_.erase(it);
  return true;
}

bool Week::hasDay(const std::string &date) const {
  return std::any_of(days_.begin(), days_.end(),
                     [&date](const Day &day) {
                       return day.getDate() == date;
                     });
}

std::size_t Week::dayCount() const { return days_.size(); }

bool Week::hasTeacher(const std::string &teacherId) const {
  return std::any_of(days_.begin(), days_.end(),
                     [&teacherId](const Day &day) {
                       return day.hasTeacher(teacherId);
                     });
}

bool Week::hasTimeConflict(const std::string &dayName, int startHour,
                           int durationHours) const {
  const auto it = std::find_if(
      days_.begin(), days_.end(), [&dayName](const Day &day) {
        return day.name() == dayName;
      });
  return it != days_.end() &&
         it->hasTimeConflict(startHour, durationHours);
}
