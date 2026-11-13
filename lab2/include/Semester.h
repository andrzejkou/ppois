#pragma once

#include "Subject.h"

#include <string>
#include <vector>

class Semester {
public:
  Semester(int number, int year);
  void addSubject(const Subject &subject);
  bool hasSubject(const std::string &subjectCode) const;
  int number() const;
  int year() const;

private:
  int number_;
  int year_;
  std::vector<Subject> subjects_;
};
