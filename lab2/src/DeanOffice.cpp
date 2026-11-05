#include "Box.h"
#include "accounting_types.h"
#include "dean_office.h"
#include "people.h"
#include "specialties.h"
#include <algorithm>
void DeanOffice::addGroup(const Group &group) {
  if (!findGroup(group.code())) {
    groups_.push_back(group);
  }
}
void Student::removeScholarship() { scholarship_.reset(); }
void Student::awardScholarship() {
  int amount = 0;
  double average = averageMarkCheck();
  if (average >= 9) {
    amount = 2000;
  } else if (average >= 8) {
    amount = 1500;
  } else if (average >= 6) {
    amount = 1200;
  } else {
    amount = 1000;
  }
  if (scholarship_) {

    scholarship_->changeAmount(amount);
  } else
    scholarship_.emplace("Академическая", amount);
}

void DeanOffice::manipulateScholarship() {
  actionForMatches(
      students_,
      [](std::pair<std::string, Student> &student) {
        return student.second.isEligibleForScholarship();
      },
      [](std::pair<std::string, Student> &student) {
        return student.second.awardScholarship();
      },
      [](std::pair<std::string, Student> &student) {
        return student.second.removeScholarship();
      });
}

bool DeanOffice::expelStudent(const std::string &studentId) {
  bool removed = false;
  for (auto &group : groups_) {
    if (group[studentId]) {
      removed = group -= studentId;
    }
  }

  return removed;
}

bool DeanOffice::enrollStudent(const std::string &groupCode,
                               const Student &student) {

  for (auto &group : groups_) {
    if (group.code() == groupCode) {
      if (group[student.studentId()]) {
        return false;
      }

      group += student;
      return true;
    }
  }
  return false;
}

bool DeanOffice::transferStudent(const std::string &fromGroup,
                                 const std::string &toGroup,
                                 const std::string &studentId) {

  Group *source = nullptr;
  Group *target = nullptr;

  source = findIn(groups_, [&fromGroup](const Group &group) {
    return group.code() == fromGroup;
  });

  target = findIn(groups_, [&toGroup](const Group &group) {
    return group.code() == toGroup;
  });

  if (source == nullptr || target == nullptr || source == target) {
    return false;
  }

  if (!(*source)[studentId] || (*target)[studentId] ||
      target->availableSeats() == 0) {
    return false;
  }

  auto it = std::find_if(
      source->students().begin(), source->students().end(),
      [&](const Student &student) { return student.studentId() == studentId; });

  if (it == source->students().end()) {
    return false;
  }

  (*target) += (std::move(*it));
  (*source) -= studentId;

  return true;
}

const Group *DeanOffice::findGroup(const std::string &groupName) const {
  return findIn(groups_, [groupName](const Group &group) {
    return group.code() == groupName;
  });
}

std::size_t DeanOffice::totalStudents() const {
  std::size_t total = 0;
  for (const auto &group : groups_) {
    total += group.size();
  }
  return total;
}
