#pragma once

#include <utility>

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
  const std::string &enrollmentDate() const { return enrollmentDate_; }
  void setEnrollmentDate(std::string value) { enrollmentDate_ = std::move(value); }
  const std::string &educationForm() const { return educationForm_; }
  void setEducationForm(std::string value) { educationForm_ = std::move(value); }
  const std::string &fundingType() const { return fundingType_; }
  void setFundingType(std::string value) { fundingType_ = std::move(value); }
  const std::string &status() const { return status_; }
  void setStatus(std::string value) { status_ = std::move(value); }
  const std::string &expulsionDate() const { return expulsionDate_; }
  void setExpulsionDate(std::string value) { expulsionDate_ = std::move(value); }

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
  std::string enrollmentDate_;
  std::string educationForm_;
  std::string fundingType_;
  std::string status_{"обучается"};
  std::string expulsionDate_;
};
