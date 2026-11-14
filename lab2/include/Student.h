#pragma once

#include "Benefit.h"
#include "Mark.h"
#include "People.h"
#include "Scholarship.h"
#include "Submission.h"
#include "StudentApplication.h"
#include "ScholarshipApplication.h"

#include <algorithm>
#include <cstddef>
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
  double averageMarkCheck() const;
  bool hasMarkForSubject(const std::string &subjectCode) const;
  double averageForSubject(const std::string &subjectCode) const;
  std::size_t removeMarksForSubject(const std::string &subjectCode);
  std::size_t countMarksBelow(int threshold) const;
  void awardScholarship();
  void assignBenefit(const Benefit &benefit);
  void removeBenefit();
  std::size_t recalculateAcademicDebtCount(int passingGrade);
  void addMark(const Mark &mark);
  void addSubmission(const Submission &submission);
  bool recordSubmissionScore(const std::string &workTitle, int score, int maxScore);
  std::size_t submissionCount() const;
  const std::vector<Submission> &submissions() const;
  void submitApplication(const StudentApplication &application);
  bool reviewApplication(const std::string &topic);
  std::size_t applicationCount() const;
  void requestScholarship(const ScholarshipApplication &application);
  bool resolveScholarshipApplication(const std::string &scholarshipName,
                                     bool approved,
                                     const std::string &reason = {});
  std::size_t scholarshipApplicationCount() const;
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
  std::vector<Submission> submissions_;
  std::vector<StudentApplication> applications_;
  std::vector<ScholarshipApplication> scholarshipApplications_;
  int academicDebtCount_{0};
};
