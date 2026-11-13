#pragma once

#include <string>
#include <vector>

class Specialty;

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
