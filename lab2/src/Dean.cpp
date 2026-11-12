
#include "dean_office.h"
#include "people.h"

#include <iostream>
#include <utility>

Dean::Dean(std::string fullName, int age, std::string employeeId,
           std::string position)
    : Worker(std::move(fullName), age, std::move(employeeId),
             std::move(position)) {}

void Dean::printInfo() const {
  Worker::printInfo();

  std::cout << "Administrative role: Dean\n";
}

void Dean::approveOrder(AcademicOrder &order) const { order.approve(*this); }

std::string Dean::role() const { return "Декан"; }
