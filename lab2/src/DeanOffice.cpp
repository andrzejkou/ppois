#include "dean_office.h"
#include "specialties.h"
#include <algorithm>
void DeanOffice::addGroup(const Group &group) {
  if (!findGroup(group.code())) {
    groups_.push_back(group);
  }
}

bool DeanOffice::expelStudent(const std::string &studentId) {
  bool removed = false;
  for (auto &group : groups_) {
    if (group.contains(studentId)) {
      removed = group.removeStudent(studentId);
    }
  }

  return removed;
}

bool DeanOffice::enrollStudent(const std::string &groupCode,
                               const Student &student) {

  for (auto &group : groups_) {
    if (group.code() == groupCode) {
      if (group.contains(student.studentId())) {
        return false;
      }

      group.enroll(student);
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

  for (auto &group : groups_) {
    if (group.code() == fromGroup) {
      source = &group;
    }

    if (group.code() == toGroup) {
      target = &group;
    }
  }

  if (source == nullptr || target == nullptr || source == target) {
    return false;
  }

  if (!source->contains(studentId) || target->contains(studentId) ||
      target->availableSeats() == 0) {
    return false;
  }

  auto it = std::find_if(
      source->students().begin(), source->students().end(),
      [&](const Student &student) { return student.studentId() == studentId; });

  if (it == source->students().end()) {
    return false;
  }

  Student student = *it;

  target->enroll(student);
  source->removeStudent(studentId);

  return true;
}

const Group *DeanOffice::findGroup(const std::string &groupName) const {
  for (const Group &group : groups_) {
    if (group.code() == groupName)
      return &group;
  }
  return nullptr;
}

std::size_t DeanOffice::totalStudents() const {
  std::size_t total = 0;
  for (const auto &group : groups_) {
    total += group.size();
  }
  return total;
}
