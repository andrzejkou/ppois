#pragma once

#include <string>

class Curriculum;

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
