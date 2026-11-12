#include "Box.h"
#include "schedule.h"
#include <iostream>
Exam::Exam(Subject subject, std::string teacherId, std::string day,
           int startHour, int durationHours, Classroom classroom,
           std::string examType)
    : Lesson(subject, teacherId, day, startHour, durationHours, classroom),
      examType_(examType) {}

void Exam::info() const {
  std::cout << "[ЭКЗАМЕН]\n";
  baseInfo();
  std::cout << "Тип экзамена: " << examType_ << '\n';
}
void Exam::examTypeSet(std::string &examTypeEx) {
  updateField(examType_, examTypeEx);
}
const std::string &Exam::examType() const { return examType_; }
