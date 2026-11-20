#pragma once

#include <utility>

#include "Worker.h"
#include "Subject.h"

#include <cstddef>
#include <string>
#include <vector>

class Teacher : public Worker {
public:
  void printInfo() const override;
  Teacher(std::string fullName, int age, std::string employeeId,
          std::string department);
  const std::string &department() const;
  void assignCourse(const std::string &courseName);
  bool removeCourse(const std::string &courseName);
  bool replaceCourse(const std::string &oldName,
                     const std::string &newName);
  void clearTeachingAssignments();
  const std::vector<std::string> &courses() const;
  void assignSubject(const Subject &subject);
  bool removeSubject(const std::string &subjectCode);
  bool teachesSubject(const std::string &subjectCode) const;
  std::size_t subjectCount() const;
  const std::vector<Subject> &subjects() const;
  std::string role() const override;
  const std::string &academicDegree() const { return academicDegree_; }
  void setAcademicDegree(std::string value) { academicDegree_ = std::move(value); }
  const std::string &officeNumber() const { return officeNumber_; }
  void setOfficeNumber(std::string value) { officeNumber_ = std::move(value); }
  int consultationHours() const { return consultationHours_; }
  void setConsultationHours(int value) { consultationHours_ = value; }
  int workloadHours() const { return workloadHours_; }
  void setWorkloadHours(int value) { workloadHours_ = value; }

private:
  std::string department_;
  std::vector<std::string> courses_;
  std::vector<Subject> subjects_;
  std::string academicDegree_;
  std::string officeNumber_;
  int consultationHours_{0};
  int workloadHours_{0};
};
