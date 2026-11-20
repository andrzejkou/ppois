#pragma once

#include <utility>

#include "Worker.h"

#include <string>

class AcademicOrder;

class Dean : public Worker {
public:
  void printInfo() const override;

  Dean(std::string fullName, int age, std::string employeeId,
       std::string position);
  void approveOrder(AcademicOrder &order) const;
  std::string role() const override;
  const std::string &appointmentDate() const { return appointmentDate_; }
  void setAppointmentDate(std::string value) { appointmentDate_ = std::move(value); }
  const std::string &responsibilityArea() const { return responsibilityArea_; }
  void setResponsibilityArea(std::string value) { responsibilityArea_ = std::move(value); }

private:
  std::string appointmentDate_;
  std::string responsibilityArea_;
};
