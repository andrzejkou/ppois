#include "People.h"
#include <iostream>
#include <stdexcept>
#include <utility>
People::People(std::string fullName, int age)
    : fullName_(std::move(fullName)), age_(age) {
  if (age < 0) {
    throw std::invalid_argument("Возраст не может быть отрицательным");
  }
}
People::People(People &&other) noexcept
    : fullName_(std::move(other.fullName_)), age_(other.age_),
      email_(std::move(other.email_)), phone_(std::move(other.phone_)) {
  other.age_ = 0;
  other.fullName_.clear();
  other.email_.clear();
  other.phone_.clear();
}
void People::changeName(std::string newName) { fullName_ = std::move(newName); }
int People::age() const { return age_; }
std::string People::role() const { return "Человек"; }
const std::string &People::fullName() const { return fullName_; }

void People::printInfo() const {
  std::cout << "ФИО: " << fullName_ << '\n'
            << "Возраст: " << age_ << '\n'
            << "Роль: " << role() << '\n';
}
