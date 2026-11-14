#include "CourseProject.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

CourseProject::CourseProject(std::string title, int maxScore,
                             std::string topic)
    : AcademicWork(std::move(title), maxScore),
      topic_(std::move(topic)) {
  if (topic_.empty()) {
    throw std::invalid_argument("Course project topic cannot be empty");
  }
}

const std::string &CourseProject::topic() const {
  return topic_;
}

void CourseProject::addMilestone(const std::string &milestone) {
  if (milestone.empty()) {
    throw std::invalid_argument("Milestone cannot be empty");
  }

  if (std::find(milestones_.begin(), milestones_.end(), milestone) !=
      milestones_.end()) {
    throw std::invalid_argument("Milestone already exists");
  }

  milestones_.push_back(milestone);
}

const std::vector<std::string> &CourseProject::milestones() const {
  return milestones_;
}

std::string CourseProject::type() const {
  return "Course project";
}
