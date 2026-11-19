#pragma once

#include <utility>

#include <string>
#include <vector>
#include <memory>
#include <cstddef>

class Student;

class Group {
public:
  Group(std::string code, std::string specialtyCode, std::size_t capacity);
  void operator+=(const Student &student);
  bool operator-=(const std::string &studentId);
  bool operator[](const std::string &studentId) const;
  void operator+=(Student &&student);
  void operator+=(const std::shared_ptr<Student> &student);
  std::size_t size() const;
  bool changeStudentSubGroup(
    const std::string& studentId,
    int subGroupNum);
  std::size_t availableSeats() const;
  bool hasCapacity() const;
  bool containsStudent(const std::string &studentId) const;
  std::shared_ptr<Student> findStudent(const std::string &studentId) const;
  std::vector<std::shared_ptr<Student>> studentsInSubGroup(int subGroupNum) const;
  std::size_t unassignedStudentCount() const;
  std::size_t assignSubGroup(int subGroupNum);
  void sortByName();
  void sortByAverageMark();
  const std::string &code() const;
  const std::string &specialtyCode() const;
  const std::vector<std::shared_ptr<Student>> &students() const;
  int admissionYear() const { return admissionYear_; }
  void setAdmissionYear(int value) { admissionYear_ = value; }
  const std::string &educationForm() const { return educationForm_; }
  void setEducationForm(std::string value) { educationForm_ = std::move(value); }
  const std::string &curatorEmployeeId() const { return curatorEmployeeId_; }
  void setCuratorEmployeeId(std::string value) { curatorEmployeeId_ = std::move(value); }

private:
  std::string code_;
  std::string specialtyCode_;
  std::size_t capacity_;
  std::vector<std::shared_ptr <Student>> students_;
  int admissionYear_{0};
  std::string educationForm_;
  std::string curatorEmployeeId_;
};
