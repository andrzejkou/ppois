#pragma once

#include "Group.h"
#include "Specialty.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class Worker;
class Student;

class DeanOffice {
public:
  explicit DeanOffice(std::string name);
  void addWorker(const std::shared_ptr<Worker> &worker);

  bool removeWorker(const std::string &employeeId);

  Worker *findWorker(const std::string &employeeId);

  const Worker *findWorker(const std::string &employeeId) const;

  std::size_t totalWorkers() const;

  const std::vector<std::shared_ptr<Worker>> &workers() const;
  void giveScholarship(std::string studentId);
  void manipulateScholarship();
  void addGroup(const Group &group);
  void registerSpecialty(const Specialty &specialty);
  bool removeSpecialty(const std::string &specialtyCode);
  const Specialty *findSpecialty(const std::string &specialtyCode) const;
  std::size_t specialtyCount() const;
  const std::vector<Specialty> &specialties() const;
  bool enrollStudent(const std::string &groupCode, const Student &student);
  bool expelStudent(const std::string &studentId);
  bool removeFromGroup(const std::string &groupCode,
                       const std::string &studentId);
  bool transferStudent(const std::string &fromGroup, const std::string &toGroup,
                       const std::string &studentId);
  const Group *findGroup(const std::string &groupCode) const;
  std::size_t totalStudents() const;
  Student *findStudent(const std::string &studentId);
  const Student *findStudent(const std::string &studentId) const;
  bool hasStudent(const std::string &studentId) const;
  std::size_t groupCount() const;
  bool groupHasCapacity(const std::string &groupCode) const;
  bool removeEmptyGroup(const std::string &groupCode);
  bool assignCuratorToGroup(const std::string &employeeId,
                            const std::string &groupCode);
  bool mergeGroups(const std::string &sourceCode,
                   const std::string &targetCode);
  std::size_t promoteEligibleStudents(const std::string &groupCode,
                                      double minimumAverage);
  std::string groupOfStudent(const std::string &studentId) const;
  std::size_t studentsWithAcademicDebts(const std::string &groupCode) const;
  const std::string &name() const;

private:
  std::string name_;
  std::unordered_map<std::string, std::shared_ptr<Student>> students_;
  std::vector<Group> groups_;
  std::vector<Specialty> specialties_;
  std::vector<std::shared_ptr<Worker>> workers_;
};
