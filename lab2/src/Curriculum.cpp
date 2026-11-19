#include "Curriculum.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

Curriculum::Curriculum(std::string code, int totalHours)
    : code_(std::move(code)), totalHours_(totalHours) {
  if (code_.empty()) {
    throw std::invalid_argument("Curriculum code cannot be empty");
  }

  if (totalHours_ <= 0) {
    throw std::invalid_argument("Curriculum total hours must be positive");
  }
}

void Curriculum::addSubject(const std::string &subject, int hours) {
  if (subject.empty()) {
    throw std::invalid_argument("Subject name cannot be empty");
  }

  if (hours <= 0) {
    throw std::invalid_argument("Subject hours must be positive");
  }

  if (containsSubject(subject)) {
    throw std::invalid_argument("Subject already exists in curriculum");
  }

  if (hours > totalHours_ - totalHours()) {
    throw std::invalid_argument("Subject hours exceed curriculum total");
  }

  subjects_.emplace_back(subject, hours);
}

bool Curriculum::containsSubject(const std::string &subject) const {
  return std::any_of(
      subjects_.begin(), subjects_.end(),
      [&subject](const auto &entry) { return entry.first == subject; });
}

int Curriculum::hoursFor(const std::string &subject) const {
  const auto it = std::find_if(
      subjects_.begin(), subjects_.end(),
      [&subject](const auto &entry) { return entry.first == subject; });

  if (it == subjects_.end()) {
    return 0;
  }

  return it->second;
}

int Curriculum::totalHours() const {
  int result = 0;
  for (const auto &entry : subjects_) {
    result += entry.second;
  }
  return result;
}

const std::string &Curriculum::code() const {
  return code_;
}
