#include "Lecture.h"

#include <iostream>

void Lecture::info() const {
  std::cout << "=== Лекция ===\n";
  baseInfo();
}
