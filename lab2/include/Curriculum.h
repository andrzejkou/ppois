#pragma once

#include <string>
#include <vector>
#include <utility>

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
