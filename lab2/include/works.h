#pragma once

#include <string>
#include <vector>

class AcademicWork {
public:
  AcademicWork(std::string title, int maxScore);
  virtual ~AcademicWork() = default;
  virtual std::string type() const;
  const std::string &title() const;
  int maxScore() const;

private:
  std::string title_;
  int maxScore_;
};

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

class LaboratoryWork : public AcademicWork {
public:
  LaboratoryWork(std::string title, int maxScore, int number);
  int number() const;
  std::string type() const override;

private:
  int number_;
};

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

class Submission {
public:
  Submission(std::string studentId, std::string workTitle);
  void recordScore(int score, int maxScore);
  bool graded() const;
  int score() const;

private:
  std::string studentId_;
  std::string workTitle_;
  int score_{-1};
};
class ScientificWork() {};
class Evaluation {
public:
  Evaluation(std::string evaluatorId, std::string comment);
  void reviseComment(const std::string &comment);
  const std::string &evaluatorId() const;
  const std::string &comment() const;

private:
  std::string evaluatorId_;
  std::string comment_;
};
