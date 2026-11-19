#include <gtest/gtest.h>

#include "Submission.h"
#include "ScholarshipApplication.h"
#include "Subject.h"
#include "Classroom.h"
#include "Day.h"
#include "Mark.h"

#include <stdexcept>

TEST(SubmissionTest, RecordsOnlyValidScores) {
    Submission submission("S001", "Course project");
    EXPECT_FALSE(submission.graded());
    submission.recordScore(8, 10);
    EXPECT_TRUE(submission.graded());
    EXPECT_EQ(submission.score(), 8);
    EXPECT_THROW(submission.recordScore(11, 10), std::out_of_range);
    EXPECT_THROW(submission.recordScore(1, 0), std::invalid_argument);
}

TEST(ScholarshipApplicationTest, ResolvesOnce) {
    ScholarshipApplication application("S001", "Academic");
    EXPECT_FALSE(application.isResolved());
    application.reject("Insufficient average");
    EXPECT_TRUE(application.isResolved());
    EXPECT_FALSE(application.isApproved());
    EXPECT_EQ(application.decisionReason(), "Insufficient average");
    EXPECT_THROW(application.approve(), std::logic_error);
}

TEST(BasicEntitiesTest, ValidateSubjectAndClassroom) {
    Subject subject("MATH", "Mathematics", 5);
    EXPECT_EQ(subject.code(), "MATH");
    Classroom room("101", 30);
    EXPECT_TRUE(room.canFit(25));
    EXPECT_FALSE(room.canFit(31));
}


TEST(MarkTest, ExposesTheStoredDay) {
    Day expectedDay("Monday", "2026-11-16");
    Subject subject("MATH", "Mathematics", 5);
    Mark mark(8, subject, expectedDay);

    EXPECT_EQ(mark.day().name(), "Monday");
    EXPECT_EQ(mark.day().getDate(), "2026-11-16");
    EXPECT_EQ(mark.subject().code(), "MATH");
    EXPECT_EQ(mark.value(), 8);
}

TEST(ClassroomTest, ExposesRoomNumberAndCapacity) {
    Classroom room("R-204", 32);

    EXPECT_EQ(room.roomNumber(), "R-204");
    EXPECT_EQ(room.capacity(), 32);
    EXPECT_TRUE(room.canFit(32));
    EXPECT_FALSE(room.canFit(33));
}
