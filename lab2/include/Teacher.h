#pragma once

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

private:
  std::string department_;
  std::vector<std::string> courses_;
  std::vector<Subject> subjects_;
};
