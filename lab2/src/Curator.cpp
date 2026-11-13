#include "Curator.h"

#include <iostream>
#include <stdexcept>
#include <utility>
Curator::Curator(std::string fullName, int age, std::string employeeId,
                 Group groupCur)
    : Worker(std::move(fullName), age, std::move(employeeId), "Куратор"),
      groupCur_(std::move(groupCur)) {
  if (groupCur.code().empty()) {
    throw std::invalid_argument("Group code cannot be empty");
  }
}
void Curator::printInfo() const {
  Worker::printInfo();

  std::cout << "Curator's group: " << groupCur_.code() << '\n';
}

const Group &Curator::groupCur() const { return groupCur_; }
void Curator::assignGroup(Group groupToCur) {
  groupCur_ = std::move(groupToCur);
}

bool Curator::transferToSubGroup(const std::string &studentId,
                                 int subGroupNumEx) {

  return groupCur_.changeStudentSubGroup(studentId, subGroupNumEx);
}
