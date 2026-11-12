#include "schedule.h"
#include <iostream>

void Lecture::info() const {
  std::cout << "[ЛЕКЦИЯ]\n";
  baseInfo();
}
