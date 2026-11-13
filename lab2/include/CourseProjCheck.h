#pragma once

#include "Lesson.h"


class CourseProjCheck : public Lesson {
public:
  using Lesson::Lesson;
  void info() const override;
};
