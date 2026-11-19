#include "Group.h"
#include "Student.h"
#include "exceptions.h"

#include <string>

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>

Group::Group(std::string code, std::string specialtyCode, std::size_t capacity)
    : code_(std::move(code)), specialtyCode_(std::move(specialtyCode)),
      capacity_(capacity) {
  if (code_.empty() || specialtyCode_.empty()) {
    throw std::invalid_argument("Group and specialty codes cannot be empty");
  }
  if (capacity_ == 0) {
    throw std::invalid_argument("Group capacity must be positive");
  }
}

void Group::operator+=(const Student &student) {
  if ((*this)[student.studentId()]) {
    throw DuplicateStudentException("Student already belongs to this group");
  }

  if (students_.size() >= capacity_) {
    throw GroupCapacityException("Group has no free seats");
  }

  students_.push_back(std::make_shared<Student>(student));
}

void Group::operator+=(const std::shared_ptr<Student> &student) {

  if (!student) {
    throw std::invalid_argument("Student pointer is null");
  }

  if ((*this)[student->studentId()]) {
    throw DuplicateStudentException("Student already belongs to this group");
  }

  if (students_.size() >= capacity_) {
    throw GroupCapacityException("Group has no free seats");
  }

  students_.push_back(student);
}

void Group::operator+=(Student &&student) {
  if ((*this)[student.studentId()]) {
    throw DuplicateStudentException("Student already belongs to this group");
  }

  if (students_.size() >= capacity_) {
    throw GroupCapacityException("Group has no free seats");
  }

  students_.push_back(std::make_shared<Student>(std::move(student)));
}
bool Group::operator-=(const std::string &studentId) {
  auto curr = std::find_if(students_.begin(), students_.end(),
                           [&](const std::shared_ptr<Student> &student) {
                             return student->studentId() == studentId;
                           });

  if (curr == students_.end()) {
    return false;
  }

  students_.erase(curr);
  return true;
}

bool Group::operator[](const std::string &studentId) const {

  for (const auto &student : students_) {

    if (student->studentId() == studentId) {
      return true;
    }
  }

  return false;
}

bool Group::changeStudentSubGroup(const std::string &studentId,
                                  int subGroupNum) {

  auto it = std::find_if(students_.begin(), students_.end(),
                         [&](const std::shared_ptr<Student> &student) {
                           return student->studentId() == studentId;
                         });

  if (it == students_.end())
    return false;

  (*it)->setSubGroup(subGroupNum);
  return true;
}

std::size_t Group::size() const { return students_.size(); }

bool Group::hasCapacity() const { return students_.size() < capacity_; }

bool Group::containsStudent(const std::string &studentId) const {
  return (*this)[studentId];
}

std::shared_ptr<Student> Group::findStudent(const std::string &studentId) const {
  const auto it = std::find_if(
      students_.begin(), students_.end(),
      [&studentId](const std::shared_ptr<Student> &student) {
        return student->studentId() == studentId;
      });
  return it == students_.end() ? nullptr : *it;
}

std::vector<std::shared_ptr<Student>>
Group::studentsInSubGroup(int subGroupNum) const {
  if (subGroupNum < 0) {
    throw std::invalid_argument("Subgroup number cannot be negative");
  }
  std::vector<std::shared_ptr<Student>> result;
  std::copy_if(students_.begin(), students_.end(),
               std::back_inserter(result),
               [subGroupNum](const std::shared_ptr<Student> &student) {
                 return student->subGroupNum() == subGroupNum;
               });
  return result;
}

std::size_t Group::unassignedStudentCount() const {
  return static_cast<std::size_t>(std::count_if(
      students_.begin(), students_.end(),
      [](const std::shared_ptr<Student> &student) {
        return student->subGroupNum() == 0;
      }));
}

std::size_t Group::assignSubGroup(int subGroupNum) {
  if (subGroupNum <= 0) {
    throw std::invalid_argument("Subgroup number must be positive");
  }
  for (const auto &student : students_) {
    student->setSubGroup(subGroupNum);
  }
  return students_.size();
}

void Group::sortByName() {
  std::sort(students_.begin(), students_.end(),
            [](const auto &left, const auto &right) {
              return left->fullName() < right->fullName();
            });
}

void Group::sortByAverageMark() {
  std::stable_sort(students_.begin(), students_.end(),
                   [](const auto &left, const auto &right) {
                     return left->averageMarkCheck() >
                            right->averageMarkCheck();
                   });
}

std::size_t Group::availableSeats() const {
  return capacity_ - students_.size();
}

const std::string &Group::code() const { return code_; }

const std::string &Group::specialtyCode() const { return specialtyCode_; }

const std::vector<std::shared_ptr<Student>> &Group::students() const {
  return students_;
}
