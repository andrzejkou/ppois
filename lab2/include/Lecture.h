#pragma once

#include "Lesson.h"


class Lecture : public Lesson {
public:
  using Lesson::Lesson;
  void info() const override;
};
