#pragma once

#include <string>

class Assignment {
public:
  Assignment(std::string studentId, std::string workTitle);
  void submit(const std::string &fileName);
  bool isSubmitted() const;
  const std::string &fileName() const;

private:
  std::string studentId_;
  std::string workTitle_;
  std::string fileName_;
};
