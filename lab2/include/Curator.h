#pragma once

#include <utility>

#include "Group.h"
#include "Worker.h"

#include <memory>
#include <string>

class Curator : public Worker {
public:
  void printInfo() const override;
  Curator(std::string fullName, int age, std::string employeeId,
          Group groupCur);
  Curator(std::string fullName, int age, std::string employeeId,
          std::shared_ptr<Group> groupCur);
  const Group &groupCur() const;
  void assignGroup(Group groupToCur);
  void assignGroup(const std::shared_ptr<Group> &groupToCur);
  bool transferToSubGroup(const std::string &studentId, int subGroupNumEx);
  bool supervises(const std::string &studentId) const;
  std::size_t studentCount() const;
  std::size_t studentsWithDebts() const;
  double groupAverageMark() const;
  std::size_t countSubGroup(int subGroupNumber) const;
  bool removeStudent(const std::string &studentId);
  std::size_t assignUnassignedStudentsToSubGroup(int subGroupNumber);
  std::size_t reassignStudentsWithDebts(int targetSubGroup);
  const std::string &appointmentDate() const { return appointmentDate_; }
  void setAppointmentDate(std::string value) { appointmentDate_ = std::move(value); }
  const std::string &consultationSchedule() const { return consultationSchedule_; }
  void setConsultationSchedule(std::string value) { consultationSchedule_ = std::move(value); }
  const std::string &officeNumber() const { return officeNumber_; }
  void setOfficeNumber(std::string value) { officeNumber_ = std::move(value); }

private:
  std::shared_ptr<Group> groupCur_;
  std::string appointmentDate_;
  std::string consultationSchedule_;
  std::string officeNumber_;
};
