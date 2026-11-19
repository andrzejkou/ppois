
#include "Secretary.h"

#include <iostream>
#include <stdexcept>
#include <utility>

Secretary::Secretary(std::string fullName, int age, std::string employeeId)
    : Worker(std::move(fullName), age, std::move(employeeId), "Секретарь") {}

void Secretary::printInfo() const {
  Worker::printInfo();

  std::cout << "Administrative role: Secretary\n";
}

std::string Secretary::registerDocument(const std::string &title) const {
  if (title.empty()) {
    throw std::invalid_argument("Document title cannot be empty");
  }

  return "Документ зарегистрирован: " + title;
}
