#include "DeanOffice.h"
#include "Box.h"
#include "Worker.h"

#include "Group.h"
#include "Student.h"
#include <algorithm>
#include <stdexcept>
#include <utility>
DeanOffice::DeanOffice(std::string name) : name_(std::move(name)) {
  if (name_.empty()) {
    throw std::invalid_argument("Dean office name cannot be empty");
  }
}

void DeanOffice::addWorker(const std::shared_ptr<Worker> &worker) {
  if (!worker) {
    throw std::invalid_argument("Worker cannot be null");
  }

  if (findWorker(worker->employeeId())) {
    throw std::invalid_argument("Worker already exists");
  }

  workers_.push_back(worker);
}

bool DeanOffice::removeWorker(const std::string &employeeId) {
  auto it = std::find_if(workers_.begin(), workers_.end(),
                         [&employeeId](const auto &worker) {
                           return worker->employeeId() == employeeId;
                         });

  if (it == workers_.end()) {
    return false;
  }

  workers_.erase(it);
  return true;
}

Worker *DeanOffice::findWorker(const std::string &employeeId) {
  auto it = std::find_if(workers_.begin(), workers_.end(),
                         [&employeeId](const auto &worker) {
                           return worker->employeeId() == employeeId;
                         });

  return it == workers_.end() ? nullptr : it->get();
}

const Worker *DeanOffice::findWorker(const std::string &employeeId) const {
  auto it = std::find_if(workers_.begin(), workers_.end(),
                         [&employeeId](const auto &worker) {
                           return worker->employeeId() == employeeId;
                         });

  return it == workers_.end() ? nullptr : it->get();
}

std::size_t DeanOffice::totalWorkers() const { return workers_.size(); }

const std::vector<std::shared_ptr<Worker>> &DeanOffice::workers() const {
  return workers_;
}

void DeanOffice::giveScholarship(std::string studentId) {
  auto it = students_.find(studentId);

  if (it == students_.end()) {
    throw std::invalid_argument("Student not found");
  }

  if (!it->second->isEligibleForScholarship()) {
    throw std::invalid_argument("Student is not eligible for scholarship");
  }

  it->second->awardScholarship();
}

void DeanOffice::addGroup(const Group &group) {
  if (!findGroup(group.code())) {
    groups_.push_back(group);
  }
}
void DeanOffice::manipulateScholarship() {
  actionForMatches(
      students_,
      [](auto &student) { return student.second->isEligibleForScholarship(); },
      [](auto &student) { return student.second->awardScholarship(); },
      [](auto &student) { return student.second->removeScholarship(); });
}

bool DeanOffice::expelStudent(const std::string &studentId) {
  bool removed = false;
  for (auto &group : groups_) {
    if (group -= studentId) {
      removed = true;
    }
  }

  if (removed) {
    students_.erase(studentId);
  }

  return removed;
}

bool DeanOffice::enrollStudent(const std::string &groupCode,
                               const Student &student) {
  if (students_.find(student.studentId()) != students_.end()) {
    return false;
  }

  for (auto &group : groups_) {
    if (group.code() == groupCode) {
      if (group[student.studentId()]) {
        return false;
      }

      auto studentPtr = std::make_shared<Student>(student);

      group += studentPtr;
      students_.emplace(student.studentId(), studentPtr);

      return true;
    }
  }

  return false;
}
bool DeanOffice::removeFromGroup(const std::string &groupCode,
                                 const std::string &studentId) {
  for (auto &group : groups_) {
    if (group.code() == groupCode) {
      if (!(group -= studentId)) {
        return false;
      }

      return true;
    }
  }

  return false;
}
const std::string &DeanOffice::name() const { return name_; }
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

Student *DeanOffice::findStudent(const std::string &studentId) {
  const auto it = students_.find(studentId);
  return it == students_.end() ? nullptr : it->second.get();
}

const Student *DeanOffice::findStudent(const std::string &studentId) const {
  const auto it = students_.find(studentId);
  return it == students_.end() ? nullptr : it->second.get();
}

bool DeanOffice::hasStudent(const std::string &studentId) const {
  return students_.find(studentId) != students_.end();
}

std::size_t DeanOffice::groupCount() const { return groups_.size(); }

bool DeanOffice::groupHasCapacity(const std::string &groupCode) const {
  const Group *group = findGroup(groupCode);
  return group != nullptr && group->availableSeats() > 0;
}

bool DeanOffice::removeEmptyGroup(const std::string &groupCode) {
  const auto it = std::find_if(groups_.begin(), groups_.end(),
      [&groupCode](const Group &group) { return group.code() == groupCode; });
  if (it == groups_.end() || it->size() != 0) return false;
  groups_.erase(it);
  return true;
}

std::string DeanOffice::groupOfStudent(const std::string &studentId) const {
  for (const auto &group : groups_)
    if (group[studentId]) return group.code();
  return {};
}

std::size_t DeanOffice::studentsWithAcademicDebts(const std::string &groupCode) const {
  const Group *group = findGroup(groupCode);
  if (!group) return 0;
  std::size_t count = 0;
  for (const auto &student : group->students())
    if (student->academicDebtCount() > 0) ++count;
  return count;
}

std::size_t DeanOffice::totalStudents() const {
  std::size_t total = 0;
  for (const auto &group : groups_) {
    total += group.size();
  }
  return total;
}

void DeanOffice::registerSpecialty(const Specialty &specialty) {
  if (findSpecialty(specialty.code())) {
    throw std::invalid_argument("Specialty already registered");
  }
  specialties_.push_back(specialty);
}

bool DeanOffice::removeSpecialty(const std::string &specialtyCode) {
  auto it = std::find_if(specialties_.begin(), specialties_.end(),
                         [&specialtyCode](const Specialty &specialty) {
                           return specialty.code() == specialtyCode;
                         });
  if (it == specialties_.end()) {
    return false;
  }
  specialties_.erase(it);
  return true;
}

const Specialty *DeanOffice::findSpecialty(
    const std::string &specialtyCode) const {
  auto it = std::find_if(specialties_.begin(), specialties_.end(),
                         [&specialtyCode](const Specialty &specialty) {
                           return specialty.code() == specialtyCode;
                         });
  return it == specialties_.end() ? nullptr : &(*it);
}

std::size_t DeanOffice::specialtyCount() const { return specialties_.size(); }

const std::vector<Specialty> &DeanOffice::specialties() const {
  return specialties_;
}
