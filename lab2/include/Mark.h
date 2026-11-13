#pragma once

#include "Day.h"
#include "Subject.h"

class Mark {
public:
  Mark(int value, Subject subject, Day day);
  const Day &day() const;
  const Subject &subject() const;
  int value() const;

private:
  int value_;
  Subject subject_;
  Day day_;
};
