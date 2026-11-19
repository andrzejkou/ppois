
#include "AcademicOrder.h"
#include "Dean.h"

#include <stdexcept>
#include <utility>

AcademicOrder::AcademicOrder(int number, std::string title)
    : number_(number), title_(std::move(title)) {
  if (number_ <= 0) {
    throw std::invalid_argument("Academic order number must be positive");
  }

  if (title_.empty()) {
    throw std::invalid_argument("Academic order title cannot be empty");
  }
}

void AcademicOrder::approve(const Dean &dean) {
  (void)dean;
  approved_ = true;
}

bool AcademicOrder::isApproved() const { return approved_; }

int AcademicOrder::number() const { return number_; }

const std::string &AcademicOrder::title() const { return title_; }
