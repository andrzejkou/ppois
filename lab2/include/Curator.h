#pragma once

#include "Worker.h"
#include "Group.h"

#include <string>

class Curator : public Worker {
public:
  void printInfo() const override;
  Curator(std::string fullName, int age, std::string employeeId,
          Group groupCur_);
  const Group &groupCur() const;
  void assignGroup(Group groupToCur);
  bool transferToSubGroup(const std::string &studentId, int subGroupNumEx);
  bool supervises(const std::string &studentId) const;
  std::size_t studentCount() const;
  std::size_t studentsWithDebts() const;
  double groupAverageMark() const;
  std::size_t countSubGroup(int subGroupNumber) const;
  bool removeStudent(const std::string &studentId);

private:
  Group groupCur_;
};
