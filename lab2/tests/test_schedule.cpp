#include <gtest/gtest.h>

#include "Schedule.h"
#include "Week.h"
#include "Day.h"
#include "Lecture.h"
#include "Classroom.h"
#include "Subject.h"

#include <memory>
#include <stdexcept>

TEST(ScheduleTest, AddsFindsAndRemovesWeeks) {
    Schedule schedule("CS-21");
    Week first(1);
    Week second(2);
    schedule.addWeek(first);
    schedule.addWeek(second);

    EXPECT_EQ(schedule.weekCount(), 2U);
    EXPECT_TRUE(schedule.hasWeek(1));
    EXPECT_EQ(schedule.weekNumbers().size(), 2U);
    EXPECT_TRUE(schedule.removeWeek(1));
    EXPECT_FALSE(schedule.hasWeek(1));
    EXPECT_EQ(schedule.removeWeeksWithoutLessons(), 1U);
}

TEST(ScheduleTest, ChecksTeacherAndTimeConflicts) {
    Schedule schedule("CS-21");
    Day monday("Monday", "2026-11-02");
    Classroom room("101", 30);
    Subject math("MATH", "Mathematics", 5);
    monday.addLesson(std::make_shared<Lecture>(math, "T001", "Monday", 10, 2, room));
    Week week(1);
    week.addDay(monday);
    schedule.addWeek(week);

    EXPECT_EQ(schedule.lessonCount(), 1U);
    EXPECT_TRUE(schedule.hasTeacher("T001"));
    EXPECT_TRUE(schedule.hasTimeConflict(1, "Monday", 11, 1));
    EXPECT_FALSE(schedule.hasTimeConflict(1, "Monday", 12, 1));
}

TEST(DayAndWeekTest, RemovesDaysAndLessons) {
    Day monday("Monday", "2026-11-02");
    EXPECT_FALSE(monday.removeLesson(0));
    Week week(1);
    week.addDay(monday);
    EXPECT_TRUE(week.hasDay("2026-11-02"));
    EXPECT_TRUE(week.removeDay("2026-11-02"));
    EXPECT_FALSE(week.hasDay("2026-11-02"));
}
