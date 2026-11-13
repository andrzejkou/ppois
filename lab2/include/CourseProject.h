#pragma once

#include "AcademicWork.h"

#include <string>
#include <vector>

class CourseProject : public AcademicWork {
public:
  CourseProject(std::string title, int maxScore, std::string topic);
  const std::string &topic() const;
  void addMilestone(const std::string &milestone);
  const std::vector<std::string> &milestones() const;
  std::string type() const override;

private:
  std::string topic_;
  std::vector<std::string> milestones_;
};
