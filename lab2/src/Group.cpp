
#include "exception.h"
#include "specialties.h"
#include <string>

#include <algorithm>
#include <utility>

Group::Group(std::string code, std::string specialtyCode, std::size_t capacity)
    : code_(std::move(code)), specialtyCode_(std::move(specialtyCode)),
      capacity_(capacity) {

  void Group::enroll(const Student &student) {
    if (contains(student.studentId())) {
      throw DuplicateStudentException("Student already belongs to this group");
    }

    if (students_.size() >= capacity_) {
      throw GroupCapacityException("Group has no free seats");
    }

    students_.push_back(student);
  }

  bool Group::removeStudent(const std::string &studentId) {
    auto curr = std::find_if(students_.begin(), students_.end(),
                             [&](const Student &student) {
                               return student.studentId() == studentId;
                             });

    if (curr == students_.end()) {
      return false;
    }

    students_.erase(curr);
    return true;
  }

  bool Group::contains(const std::string &studentId) const {
    for (const auto &student : students_) {
      if (student.studentId() == studentId) {
        return true;
      }
    }

    return false;
  }

  std::size_t Group::size() const { return students_.size(); }

  std::size_t Group::availableSeats() const {
    return capacity_ - students_.size();
  }

  const std::string &Group::code() const { return code_; }

  const std::string &Group::specialtyCode() const { return specialtyCode_; }

  const std::vector<Student> &Group::students() const { return students_; }
