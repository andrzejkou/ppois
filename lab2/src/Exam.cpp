#include "Exam.h"

#include <iostream>
#include <stdexcept>
#include <utility>

Exam::Exam(Subject subject, std::string teacherId, std::string day,
           int startHour, int durationHours, Classroom classroom,
           std::string examType)
    : Lesson(std::move(subject), std::move(teacherId), std::move(day),
             startHour, durationHours, std::move(classroom)),
      examType_(std::move(examType)) {

  if (examType_.empty()) {
    throw std::invalid_argument("Exam type cannot be empty");
  }
}

const std::string &Exam::examType() const { return examType_; }

void Exam::info() const {
  std::cout << "=== Экзамен ===\n";
  baseInfo();
  std::cout << "Тип экзамена: " << examType_ << '\n';
}

void Exam::examTypeSet(std::string &examTypeEx) {
  if (examTypeEx.empty()) {
    throw std::invalid_argument("Exam type cannot be empty");
  }

  examType_ = std::move(examTypeEx);
}
