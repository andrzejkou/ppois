#pragma once

#include "Worker.h"

#include <string>
#include <vector>

class Teacher : public Worker {
public:
  void printInfo() const override;
  Teacher(std::string fullName, int age, std::string employeeId,
          std::string department);
  const std::string &department() const;
  void assignCourse(const std::string &courseName);
  const std::vector<std::string> &courses() const;
  std::string role() const override;

private:
  std::string department_;
  std::vector<std::string> courses_;
};
