#pragma once

#include "Group.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Student;

class DeanOffice {
public:
  explicit DeanOffice(std::string name);
  void giveScholarship(std::string studentId);
  void manipulateScholarship();
  void addGroup(const Group &group);
  bool enrollStudent(const std::string &groupCode, const Student &student);
  bool expelStudent(const std::string &studentId);
  bool removeFromGroup(const std::string &groupCode,
                       const std::string &studentId);
  bool transferStudent(const std::string &fromGroup, const std::string &toGroup,
                       const std::string &studentId);
  const Group *findGroup(const std::string &groupCode) const;
  std::size_t totalStudents() const;
  const std::string &name() const;

private:
  std::string name_;
  std::unordered_map<std::string, std::shared_ptr<Student>> students_;
  std::vector<Group> groups_;
};
