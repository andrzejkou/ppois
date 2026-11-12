#pragma once
#include "accounting_types.h"
#include "specialties.h"
#include "works.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>
class AcademicOrder;
class People {
public:
  People(std::string fullName, int age);
  People(People &&other) noexcept;
  People(const People &) = default;
  People &operator=(const People &) = default;
  People &operator=(People &&) noexcept = default;
  virtual ~People() = default;
  virtual void printInfo() const;
  const std::string &fullName() const;
  int age() const;

  virtual std::string role() const;
  void changeName(std::string newName);

protected:
  std::string fullName_;
  int age_;
};

class Student : public People {
public:
  Student(std::string fullName, int age, std::string studentId, int year);
  Student(Student &&other) noexcept;
  Student(const Student &) = default;
  Student &operator=(const Student &) = default;
  Student &operator=(Student &&) noexcept = default;
  const std::string &studentId() const;
  int year() const;
  void averageMarkMake();
  double averageMarkCheck();
  void awardScholarship();
  void addMark(const Mark &mark);
  void removeScholarship();
  void printInfo() const override;
  void promoteToNextYear();
  void setAcademicDebtCount(int count);
  int academicDebtCount() const;
  bool isEligibleForScholarship() const;
  std::string role() const override;
  int subGroupNum() const;
  void setSubGroup(int subGroupNumEx);

private:
  std::string studentId_;
  int subGroupNum_;
  int year_;
  double averageMark;
  std::optional<Scholarship> scholarship_;
  std::optional<Benefit> benefit_;
  std::vector<Mark> marks_;
  int academicDebtCount_{0};
};

class Worker : public People {
public:
  Worker(std::string fullName, int age, std::string employeeId,
         std::string position);
  void printInfo() const override;

  const std::string &employeeId() const;
  const std::string &position() const;
  void changePosition(const std::string &position);
  std::string role() const override;

protected:
  std::string employeeId_;
  std::string position_;
};

class Dean : public Worker {
public:
  void printInfo() const override;

  Dean(std::string fullName, int age, std::string employeeId,
       std::string position);
  void approveOrder(AcademicOrder &order) const;

  std::string role() const override;
};

class Secretary : public Worker {
public:
  void printInfo() const override;
  Secretary(std::string fullName, int age, std::string employeeId);
  std::string registerDocument(const std::string &title) const;
};

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

class Curator : public Worker {
public:
  void printInfo() const override;
  Curator(std::string fullName, int age, std::string employeeId,
          Group groupCur_);
  const Group &groupCur() const;
  void assignGroup(Group groupToCur);
  bool transferToSubGroup(const std::string &studentId, int subGroupNumEx);

private:
  Group groupCur_;
};
class Company {
public:
  Company(std::string typeOfContract, std::string name);
  const std::string &contractType() const;
  const std::string &name() const;

  void addPartner(const std::shared_ptr<Company> &company);
  bool removePartner(const std::string &companyName);
  bool hasPartner(const std::string &companyName) const;
  void printInfo() const;

private:
  std::string typeOfContract_;
  std::string name_;
  std::vector<std::shared_ptr<Company>> integrCompanies_;
};
