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

private:
  Group groupCur_;
};
