#pragma once

#include "Lesson.h"


class Seminar : public Lesson {
public:
  using Lesson::Lesson;
  void info() const override;
};
