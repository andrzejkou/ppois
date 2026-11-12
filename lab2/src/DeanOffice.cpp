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

void DeanOffice::manipulateScholarship() {
  actionForMatches(
      students_,
      [](auto &student) { return student.second.isEligibleForScholarship(); },
      [](auto &student) { return student.second.awardScholarship(); },
      [](auto &student) { return student.second.removeScholarship(); });
}

bool DeanOffice::expelStudent(const std::string &studentId) {
  bool removed = false;
  for (auto &group : groups_) {
    if (group -= studentId) {
      removed = true;
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

  auto it =
      findIn(source->students(), [&](const std::shared_ptr<Student> &student) {
        return student->studentId() == studentId;
      });

  if (it == nullptr) {
    return false;
  }

  (*target) += (*it);
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
