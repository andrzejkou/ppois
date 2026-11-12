#include "schedule.h"
#include <iostream>

Perezdacha::Perezdacha(Subject subject, std::string teacherId, std::string day,
                       int startHour, int durationHours, Classroom classroom,
                       std::string examType, int number)
    : Exam(subject, teacherId, day, startHour, durationHours, classroom,
           examType),
      number_(number) {}

int Perezdacha::number() { return number_; }

void Perezdacha::info() const {
  std::cout << "Пересдача №" << number_ << '\n';
  baseInfo();
  std::cout << "Тип экзамена: " << examType() << '\n';
}
