#include "Consultation.h"

#include <iostream>
#include <utility>

Consultation::Consultation(Subject subject, std::string teacherId,
                           std::string day, int startHour, int durationHours,
                           Classroom classroom, Exam consExam)
    : Lesson(std::move(subject), std::move(teacherId), std::move(day),
             startHour, durationHours, std::move(classroom)),
      consExam_(std::move(consExam)) {}

const Exam &Consultation::consExam() const { return consExam_; }

void Consultation::info() const {
  std::cout << "=== Консультация ===\n";
  baseInfo();
  std::cout << "Экзамен: " << consExam_.examType() << '\n';
}
