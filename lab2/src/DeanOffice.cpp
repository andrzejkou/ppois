#include "DeanOffice.h"
#include "Box.h"
#include "Curator.h"
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
    groups_.push_back(std::make_shared<Group>(group));
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
  auto studentIt = students_.find(studentId);

  if (studentIt == students_.end()) {
    return false;
  }

  // Удаляем студента из всех групп, если он там состоит.
  for (auto &group : groups_) {
    *group -= studentId;
  }

  // Полностью удаляем запись из реестра.
  students_.erase(studentIt);

  return true;
}

bool DeanOffice::enrollStudent(const std::string &groupCode,
                               const Student &student) {
  auto group = std::find_if(
      groups_.begin(), groups_.end(),
      [&groupCode](const std::shared_ptr<Group> &item) { return item->code() == groupCode; });

  if (group == groups_.end()) {
    return false;
  }

  // Студент не может одновременно состоять в нескольких группах.
  if (!groupOfStudent(student.studentId()).empty()) {
    return false;
  }

  // Если студент уже зарегистрирован, используем существующий объект.
  auto studentIt = students_.find(student.studentId());

  std::shared_ptr<Student> studentPtr;

  if (studentIt != students_.end()) {
    studentPtr = studentIt->second;
  } else {
    studentPtr = std::make_shared<Student>(student);
  }

  // Сначала добавляем студента в группу.
  // Если группа заполнена, исключение не изменит реестр.
  **group += studentPtr;

  // Регистрируем только нового студента.
  if (studentIt == students_.end()) {
    students_.emplace(student.studentId(), studentPtr);
  }

  return true;
}

bool DeanOffice::removeFromGroup(const std::string &groupCode,
                                 const std::string &studentId) {
  for (auto &group : groups_) {
    if (group->code() == groupCode) {
      if (!(*group -= studentId)) {
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
  auto sourceIt = std::find_if(groups_.begin(), groups_.end(),
      [&fromGroup](const auto &group) { return group->code() == fromGroup; });
  auto targetIt = std::find_if(groups_.begin(), groups_.end(),
      [&toGroup](const auto &group) { return group->code() == toGroup; });

  if (sourceIt == groups_.end() || targetIt == groups_.end() ||
      sourceIt == targetIt) {
    return false;
  }

  Group &source = **sourceIt;
  Group &target = **targetIt;
  const auto student = source.findStudent(studentId);
  if (!student || target[studentId] || !target.hasCapacity()) {
    return false;
  }

  target += student;
  source -= studentId;
  return true;
}

const Group *DeanOffice::findGroup(const std::string &groupName) const {
  const auto it = std::find_if(groups_.begin(), groups_.end(),
      [&groupName](const auto &group) { return group->code() == groupName; });
  return it == groups_.end() ? nullptr : it->get();
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

bool DeanOffice::assignCuratorToGroup(const std::string &employeeId,
                                      const std::string &groupCode) {
  Worker *worker = findWorker(employeeId);
  auto groupIt = std::find_if(groups_.begin(), groups_.end(),
      [&groupCode](const auto &group) { return group->code() == groupCode; });
  auto *curator = dynamic_cast<Curator *>(worker);
  if (!curator || groupIt == groups_.end()) {
    return false;
  }

  // Убираем отметку о назначении с предыдущей группы этого куратора.
  for (const auto &group : groups_) {
    if (group->curatorEmployeeId() == employeeId) {
      group->setCuratorEmployeeId({});
    }
  }

  curator->assignGroup(*groupIt);
  (*groupIt)->setCuratorEmployeeId(employeeId);
  return true;
}

bool DeanOffice::mergeGroups(const std::string &sourceCode,
                             const std::string &targetCode) {
  if (sourceCode == targetCode) {
    return false;
  }

  auto sourceIt = std::find_if(groups_.begin(), groups_.end(),
      [&sourceCode](const auto &group) { return group->code() == sourceCode; });
  auto targetIt = std::find_if(groups_.begin(), groups_.end(),
      [&targetCode](const auto &group) { return group->code() == targetCode; });

  if (sourceIt == groups_.end() || targetIt == groups_.end()) {
    return false;
  }

  Group &source = **sourceIt;
  Group &target = **targetIt;
  if (source.specialtyCode() != target.specialtyCode() ||
      target.availableSeats() < source.size()) {
    return false;
  }

  const auto studentsToMove = source.students();
  for (const auto &student : studentsToMove) {
    if (target[student->studentId()]) {
      return false;
    }
  }
  for (const auto &student : studentsToMove) {
    target += student;
  }
  for (const auto &student : studentsToMove) {
    source -= student->studentId();
  }
  return true;
}

std::size_t DeanOffice::promoteEligibleStudents(const std::string &groupCode,
                                                double minimumAverage) {
  if (minimumAverage < 0.0 || minimumAverage > 10.0) {
    throw std::invalid_argument("Minimum average must be between 0 and 10");
  }
  auto group = std::find_if(
      groups_.begin(), groups_.end(),
      [&groupCode](const std::shared_ptr<Group> &item) { return item->code() == groupCode; });
  if (group == groups_.end()) {
    return 0;
  }

  std::size_t promoted = 0;
  for (const auto &student : (*group)->students()) {
    if (student->academicDebtCount() == 0 &&
        student->averageMarkCheck() >= minimumAverage) {
      student->promoteToNextYear();
      ++promoted;
    }
  }
  return promoted;
}

bool DeanOffice::removeEmptyGroup(const std::string &groupCode) {
  const auto it = std::find_if(
      groups_.begin(), groups_.end(),
      [&groupCode](const auto &group) { return group->code() == groupCode; });
  if (it == groups_.end() || (*it)->size() != 0)
    return false;
  groups_.erase(it);
  return true;
}

std::string DeanOffice::groupOfStudent(const std::string &studentId) const {
  for (const auto &group : groups_)
    if ((*group)[studentId])
      return group->code();
  return {};
}

std::size_t
DeanOffice::studentsWithAcademicDebts(const std::string &groupCode) const {
  const Group *group = findGroup(groupCode);
  if (!group)
    return 0;
  std::size_t count = 0;
  for (const auto &student : group->students())
    if (student->academicDebtCount() > 0)
      ++count;
  return count;
}

std::size_t DeanOffice::totalStudents() const {
  std::size_t total = 0;
  for (const auto &group : groups_) {
    total += group->size();
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

const Specialty *
DeanOffice::findSpecialty(const std::string &specialtyCode) const {
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
