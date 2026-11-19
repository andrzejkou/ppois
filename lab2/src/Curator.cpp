#include "Curator.h"

#include "Student.h"
#include <iostream>
#include <stdexcept>
#include <utility>
Curator::Curator(std::string fullName, int age, std::string employeeId,
                 Group groupCur)
    : Curator(std::move(fullName), age, std::move(employeeId),
              std::make_shared<Group>(std::move(groupCur))) {}

Curator::Curator(std::string fullName, int age, std::string employeeId,
                 std::shared_ptr<Group> groupCur)
    : Worker(std::move(fullName), age, std::move(employeeId), "Куратор"),
      groupCur_(std::move(groupCur)) {
  if (!groupCur_) {
    throw std::invalid_argument("Curator group cannot be null");
  }
}

void Curator::printInfo() const {
  Worker::printInfo();
  std::cout << "Curator's group: " << groupCur_->code() << '\n';
}

const Group &Curator::groupCur() const { return *groupCur_; }

void Curator::assignGroup(Group groupToCur) {
  groupCur_ = std::make_shared<Group>(std::move(groupToCur));
}

void Curator::assignGroup(const std::shared_ptr<Group> &groupToCur) {
  if (!groupToCur) {
    throw std::invalid_argument("Curator group cannot be null");
  }
  groupCur_ = groupToCur;
}

bool Curator::supervises(const std::string &studentId) const {
  return (*groupCur_)[studentId];
}

std::size_t Curator::studentCount() const { return groupCur_->size(); }

std::size_t Curator::studentsWithDebts() const {
  std::size_t count = 0;
  for (const auto &student : groupCur_->students())
    if (student->academicDebtCount() > 0)
      ++count;
  return count;
}

double Curator::groupAverageMark() const {
  if (groupCur_->students().empty())
    return 0.0;
  double total = 0.0;
  for (const auto &student : groupCur_->students())
    total += student->averageMarkCheck();
  return total / static_cast<double>(groupCur_->size());
}

std::size_t Curator::countSubGroup(int subGroupNumber) const {
  std::size_t count = 0;
  for (const auto &student : groupCur_->students())
    if (student->subGroupNum() == subGroupNumber)
      ++count;
  return count;
}

bool Curator::removeStudent(const std::string &studentId) {
  return (*groupCur_) -= studentId;
}

bool Curator::transferToSubGroup(const std::string &studentId,
                                 int subGroupNumEx) {
  return groupCur_->changeStudentSubGroup(studentId, subGroupNumEx);
}

std::size_t Curator::assignUnassignedStudentsToSubGroup(int subGroupNumber) {
  if (subGroupNumber <= 0) {
    throw std::invalid_argument("Subgroup number must be positive");
  }
  std::size_t assigned = 0;
  for (const auto &student : groupCur_->students()) {
    if (student->subGroupNum() == 0) {
      student->setSubGroup(subGroupNumber);
      ++assigned;
    }
  }
  return assigned;
}

std::size_t Curator::reassignStudentsWithDebts(int targetSubGroup) {
  if (targetSubGroup <= 0) {
    throw std::invalid_argument("Target subgroup number must be positive");
  }
  std::size_t reassigned = 0;
  for (const auto &student : groupCur_->students()) {
    if (student->academicDebtCount() > 0 &&
        student->subGroupNum() != targetSubGroup) {
      student->setSubGroup(targetSubGroup);
      ++reassigned;
    }
  }
  return reassigned;
}
