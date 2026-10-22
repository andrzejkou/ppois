#include <dean_office.h>
void DeanOffice::addGroup(const Group &group) {
  if (!findGroup(group.code())) {
    groups_.push_back(group);
  }
}

bool DeanOffice::expelStudent(const std::string &studentId) {
  auto it = students_.find(studentId);

  if (it == students_.end()) {
    return false;
  }

  for (auto &group : groups_) {
    group.removeStudent(studentId);
  }

  students_.erase(it);
  return true;
}
