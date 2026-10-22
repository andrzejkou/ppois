#pragma once

#include "people.h"
#include <string>
#include <utility>
#include <vector>

class Curriculum {
public:
  Curriculum(std::string code, int totalHours);
  void addSubject(const std::string &subject, int hours);
  bool containsSubject(const std::string &subject) const;
  int hoursFor(const std::string &subject) const;
  int totalHours() const;
  const std::string &code() const;

private:
  std::string code_;
  int totalHours_;
  std::vector<std::pair<std::string, int>> subjects_;
};

class Specialty {
public:
  Specialty(std::string code, std::string name, int durationYears);
  void attachCurriculum(const Curriculum &curriculum);
  bool hasCurriculum() const;
  const std::string &code() const;
  const std::string &name() const;
  int durationYears() const;

private:
  std::string code_;
  std::string name_;
  int durationYears_;
  bool hasCurriculum_{false};
  std::string curriculumCode_;
};

class StudyProgram {
public:
  StudyProgram(std::string name, int admissionYear);
  void addSpecialty(const Specialty &specialty);
  bool offers(const std::string &specialtyCode) const;
  int admissionYear() const;
  const std::string &name() const;

private:
  std::string name_;
  int admissionYear_;
  std::vector<std::string> specialtyCodes_;
};

class Group {
public:
  Group(std::string code, std::string specialtyCode, std::size_t capacity);
  void enroll(const Student &student);
  bool removeStudent(const std::string &studentId);
  bool contains(const std::string &studentId) const;
  std::size_t size() const;
  std::size_t availableSeats() const;
  const std::string &code() const;
  const std::string &specialtyCode() const;
  const std::vector<Student> &students() const;

private:
  std::string code_;
  std::string specialtyCode_;
  std::size_t capacity_;
  std::vector<std::string> studentIds_;
};
