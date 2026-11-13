#include "Subject.h"

#include <stdexcept>
#include <utility>

Subject::Subject(std::string code, std::string name, int credits)
    : code_(std::move(code)), name_(std::move(name)), credits_(credits) {

  if (code_.empty()) {
    throw std::invalid_argument("Subject code cannot be empty");
  }

  if (name_.empty()) {
    throw std::invalid_argument("Subject name cannot be empty");
  }

  if (credits_ <= 0) {
    throw std::invalid_argument("Subject credits must be positive");
  }
}

const std::string &Subject::code() const { return code_; }

const std::string &Subject::name() const { return name_; }

int Subject::credits() const { return credits_; }
