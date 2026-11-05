
#include "exceptions.h"
#include "people.h"
#include "specialties.h"
#include <string>

#include <algorithm>
#include <utility>

Group::Group(std::string code, std::string specialtyCode, std::size_t capacity)
    : code_(std::move(code)), specialtyCode_(std::move(specialtyCode)),
      capacity_(capacity) {}

void Group::operator+=(const Student &student) {
  if ((*this)[student.studentId()]) {
    throw DuplicateStudentException("Student already belongs to this group");
  }

  if (students_.size() >= capacity_) {
    throw GroupCapacityException("Group has no free seats");
  }

  students_.push_back(student);
}
void Group::operator+=(Student &&student) {
  students_.push_back(std::move(student));
}
bool Group::operator-=(const std::string &studentId) {
  auto curr = std::find_if(
      students_.begin(), students_.end(),
      [&](const Student &student) { return student.studentId() == studentId; });

  if (curr == students_.end()) {
    return false;
  }

  students_.erase(curr);
  return true;
}

bool Group::operator[](const std::string &studentId) const {

  for (const auto &student : students_) {

    if (student.studentId() == studentId) {
      return true;
    }
  }

  return false;
}

bool Group::changeStudentSubGroup(
    const std::string& studentId,
    int subGroupNum) {

    auto it = std::find_if(
        students_.begin(),
        students_.end(),
        [&](const Student& student) {
            return student.studentId() == studentId;
        });

    if (it == students_.end())
        return false;

    it->setSubGroup(subGroupNum);
    return true;
}

std::size_t Group::size() const { return students_.size(); }

std::size_t Group::availableSeats() const {
  return capacity_ - students_.size();
}

const std::string &Group::code() const { return code_; }

const std::string &Group::specialtyCode() const { return specialtyCode_; }

const std::vector<Student> &Group::students() const { return students_; }
