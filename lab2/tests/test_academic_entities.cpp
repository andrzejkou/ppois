#include <gtest/gtest.h>

#include "Submission.h"
#include "ScholarshipApplication.h"
#include "Subject.h"
#include "Classroom.h"

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
