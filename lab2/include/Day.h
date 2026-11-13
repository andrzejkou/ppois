#pragma once

#include "Lesson.h"

#include <memory>
#include <string>
#include <vector>

class Day {
public:
  Day(std::string name, std::string date);

  const std::string &name() const;
  const std::string &getDate() const;

  void addLesson(const std::shared_ptr<Lesson> &lesson);
  std::size_t lessonCount() const;

private:
  std::string name_;
  std::string date_;
  std::vector<std::shared_ptr<Lesson>> lessons_;
};
