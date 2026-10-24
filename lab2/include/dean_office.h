#pragma once

#include "people.h"
#include "specialties.h"
#include <string>
#include <unordered_map>
#include <vector>
class AcademicOrder {
public:
  AcademicOrder(int number, std::string title);
  void approve(const Dean &dean);
  bool isApproved() const;
  int number() const;
  const std::string &title() const;

private:
  int number_;
  std::string title_;
  bool approved_{false};
};

class TransferOrder : public AcademicOrder {
public:
  TransferOrder(int number, std::string studentId, std::string fromGroup,
                std::string toGroup);
  const std::string &studentId() const;
  const std::string &fromGroup() const;
  const std::string &toGroup() const;

private:
  std::string studentId_;
  std::string fromGroup_;
  std::string toGroup_;
};

class ExpulsionOrder : public AcademicOrder {
public:
  ExpulsionOrder(int number, std::string studentId, std::string reason);
  const std::string &studentId() const;
  const std::string &reason() const;

private:
  std::string studentId_;
  std::string reason_;
};

class StudentApplication {
public:
  StudentApplication(std::string studentId, std::string topic);
  void markReviewed();
  bool reviewed() const;
  const std::string &studentId() const;
  const std::string &topic() const;

private:
  std::string studentId_;
  std::string topic_;
  bool reviewed_{false};
};

class DeanOffice {
public:
  explicit DeanOffice(std::string name);
  void addGroup(const Group &group);
  bool enrollStudent(const std::string &groupCode, const Student &student);
  bool expelStudent(const std::string &studentId);
  bool removeFromGroup(const std::string &groupCode,
                       const std::string &studentId);
  bool transferStudent(const std::string &fromGroup, const std::string &toGroup,
                       const std::string &studentId);
  const Group *findGroup(const std::string &groupCode) const;
  std::size_t totalStudents() const;
  const std::string &name() const;

private:
  std::unordered_map<std::string, Student> students_;
  std::vector<Group> groups_;
};

class Contract {
public:
  void makeContract();

private:
  std::vector<Company> company;
};
