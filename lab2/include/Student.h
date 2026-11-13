#pragma once

#include "Benefit.h"
#include "Mark.h"
#include "People.h"
#include "Scholarship.h"

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

class Student : public People {
public:
  Student(std::string fullName, int age, std::string studentId, int year);
  Student(Student &&other) noexcept;
  Student(const Student &) = default;
  Student &operator=(const Student &) = default;
  Student &operator=(Student &&) noexcept = default;
  const std::string &studentId() const;
  int year() const;
  void averageMarkMake();
  double averageMarkCheck();
  void awardScholarship();
  void addMark(const Mark &mark);
  void removeScholarship();
  void printInfo() const override;
  void promoteToNextYear();
  void setAcademicDebtCount(int count);
  int academicDebtCount() const;
  bool isEligibleForScholarship() const;
  std::string role() const override;
  int subGroupNum() const;
  void setSubGroup(int subGroupNumEx);

private:
  std::string studentId_;
  int subGroupNum_;
  int year_;
  double averageMark;
  std::optional<Scholarship> scholarship_;
  std::optional<Benefit> benefit_;
  std::vector<Mark> marks_;
  int academicDebtCount_{0};
};
