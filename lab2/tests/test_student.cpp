#include <gtest/gtest.h>

#include "Student.h"
#include "Subject.h"
#include "Day.h"
#include "Mark.h"
#include "Submission.h"
#include "ScholarshipApplication.h"

#include <stdexcept>

TEST(StudentTest, CalculatesAverageAndFiltersMarks) {
    Student student("Ivan Ivanov", 20, "S001", 2);
    Subject math("MATH", "Mathematics", 5);
    Subject physics("PHYS", "Physics", 4);
    Day day("Monday", "2026-11-02");

    student.addMark(Mark(8, math, day));
    student.addMark(Mark(6, math, day));
    student.addMark(Mark(4, physics, day));

    EXPECT_DOUBLE_EQ(student.averageMarkCheck(), 6.0);
    EXPECT_TRUE(student.hasMarkForSubject("MATH"));
    EXPECT_DOUBLE_EQ(student.averageForSubject("MATH"), 7.0);
    EXPECT_EQ(student.countMarksBelow(6), 1U);
    EXPECT_EQ(student.recalculateAcademicDebtCount(5), 1U);
    EXPECT_EQ(student.academicDebtCount(), 1);
}

TEST(StudentTest, RemovesSubjectMarksAndRecalculatesAverage) {
    Student student("Ivan Ivanov", 20, "S001", 2);
    Subject math("MATH", "Mathematics", 5);
    Subject physics("PHYS", "Physics", 4);
    Day day("Monday", "2026-11-02");
    student.addMark(Mark(8, math, day));
    student.addMark(Mark(4, physics, day));

    EXPECT_EQ(student.removeMarksForSubject("MATH"), 1U);
    EXPECT_DOUBLE_EQ(student.averageMarkCheck(), 4.0);
    EXPECT_FALSE(student.hasMarkForSubject("MATH"));
}

TEST(StudentTest, RejectsInvalidThresholdsAndSubgroups) {
    Student student("Ivan Ivanov", 20, "S001", 2);
    EXPECT_THROW(student.countMarksBelow(11), std::invalid_argument);
    EXPECT_THROW(student.recalculateAcademicDebtCount(-1), std::invalid_argument);
    EXPECT_THROW(student.setSubGroup(-1), std::invalid_argument);
}

TEST(StudentTest, ManagesSubmissionsAndScores) {
    Student student("Ivan Ivanov", 20, "S001", 2);
    student.addSubmission(Submission("S001", "Lab work 1"));

    EXPECT_EQ(student.submissionCount(), 1U);
    EXPECT_TRUE(student.recordSubmissionScore("Lab work 1", 8, 10));
    EXPECT_FALSE(student.recordSubmissionScore("Unknown", 8, 10));
    EXPECT_TRUE(student.submissions().front().graded());
    EXPECT_EQ(student.submissions().front().score(), 8);
}

TEST(StudentTest, ManagesScholarshipApplications) {
    Student student("Ivan Ivanov", 20, "S001", 2);
    student.requestScholarship(ScholarshipApplication("S001", "Academic"));

    EXPECT_EQ(student.scholarshipApplicationCount(), 1U);
    EXPECT_TRUE(student.resolveScholarshipApplication("Academic", true));
    EXPECT_FALSE(student.resolveScholarshipApplication("Academic", true));
}
