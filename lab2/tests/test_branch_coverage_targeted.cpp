#include <gtest/gtest.h>

#include "AcademicWork.h"
#include "Benefit.h"
#include "Classroom.h"
#include "Company.h"
#include "Day.h"
#include "exceptions.h"
#include "LaboratoryWork.h"
#include "Mark.h"
#include "Student.h"
#include "Submission.h"
#include "Subject.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace {
Subject targetedSubject() { return Subject("TST", "Test subject", 5); }
Day targetedDay() { return Day("Monday", "2026-11-02"); }
}

TEST(TargetedBranchCoverage, MarkAcceptsBothGradeBoundariesAndRejectsOutside) {
  EXPECT_NO_THROW(Mark(0, targetedSubject(), targetedDay()));
  EXPECT_NO_THROW(Mark(10, targetedSubject(), targetedDay()));
  EXPECT_THROW(Mark(-1, targetedSubject(), targetedDay()), InvalidGradeException);
  EXPECT_THROW(Mark(11, targetedSubject(), targetedDay()), InvalidGradeException);
}

TEST(TargetedBranchCoverage, LaboratoryWorkValidatesBaseAndOwnFields) {
  EXPECT_THROW(LaboratoryWork("", 10, 1), std::invalid_argument);
  EXPECT_THROW(LaboratoryWork("Lab", 0, 1), std::invalid_argument);
  EXPECT_THROW(LaboratoryWork("Lab", 10, 0), std::invalid_argument);
  EXPECT_THROW(LaboratoryWork("Lab", 10, -1), std::invalid_argument);

  LaboratoryWork work("Pointers", 20, 3);
  EXPECT_EQ(work.title(), "Pointers");
  EXPECT_EQ(work.maxScore(), 20);
  EXPECT_EQ(work.number(), 3);
  EXPECT_EQ(work.type(), "Laboratory work");
}

TEST(TargetedBranchCoverage, SubmissionValidatesIdentityAndScoreBoundaries) {
  EXPECT_THROW(Submission("", "Work"), std::invalid_argument);
  EXPECT_THROW(Submission("S1", ""), std::invalid_argument);

  Submission submission("S1", "Work");
  EXPECT_FALSE(submission.graded());
  EXPECT_EQ(submission.score(), -1);
  EXPECT_THROW(submission.recordScore(1, 0), std::invalid_argument);
  EXPECT_THROW(submission.recordScore(-1, 10), std::out_of_range);
  EXPECT_THROW(submission.recordScore(11, 10), std::out_of_range);

  submission.recordScore(0, 10);
  EXPECT_TRUE(submission.graded());
  EXPECT_EQ(submission.score(), 0);
  submission.recordScore(10, 10);
  EXPECT_EQ(submission.score(), 10);
}

TEST(TargetedBranchCoverage, CompanyPartnerValidationAndRemovalPaths) {
  auto first = std::make_shared<Company>("Internship", "First");
  auto second = std::make_shared<Company>("Internship", "Second");

  EXPECT_THROW(first->addPartner(nullptr), std::invalid_argument);
  EXPECT_THROW(first->addPartner(first), std::invalid_argument);
  EXPECT_FALSE(first->hasPartner("Second"));

  first->addPartner(second);
  EXPECT_TRUE(first->hasPartner("Second"));
  EXPECT_THROW(first->addPartner(second), std::invalid_argument);
  EXPECT_FALSE(first->removePartner("Missing"));
  EXPECT_TRUE(first->removePartner("Second"));
  EXPECT_FALSE(first->hasPartner("Second"));
}

TEST(TargetedBranchCoverage, BenefitAssignmentChecksEmptyAndDuplicateIds) {
  Benefit benefit("Social", 100, "Social support");
  EXPECT_FALSE(benefit.assignedTo("S1"));
  EXPECT_THROW(benefit.assignTo(""), std::invalid_argument);

  benefit.assignTo("S1");
  EXPECT_TRUE(benefit.assignedTo("S1"));
  EXPECT_FALSE(benefit.assignedTo("S2"));
  EXPECT_THROW(benefit.assignTo("S1"), std::invalid_argument);
  benefit.assignTo("S2");
  EXPECT_TRUE(benefit.assignedTo("S2"));
}

TEST(TargetedBranchCoverage, StudentScholarshipAmountBranches) {
  Student student("Student", 20, "SCH-1", 2);
  const Subject subject = targetedSubject();
  const Day day = targetedDay();

  // Each average selects a different amount tier inside awardScholarship().
  student.addMark(Mark(5, subject, day));
  student.awardScholarship();

  student.removeMarksForSubject("TST");
  student.addMark(Mark(6, subject, day));
  student.awardScholarship();

  student.removeMarksForSubject("TST");
  student.addMark(Mark(8, subject, day));
  student.awardScholarship();

  student.removeMarksForSubject("TST");
  student.addMark(Mark(9, subject, day));
  student.awardScholarship();

  // Calling again covers the already-existing scholarship branch.
  student.awardScholarship();
}

TEST(TargetedBranchCoverage, ClassroomCapacityBoundaryCases) {
  Classroom room("T-101", 20);
  EXPECT_TRUE(room.canFit(0));
  EXPECT_TRUE(room.canFit(20));
  EXPECT_FALSE(room.canFit(21));
  EXPECT_THROW(room.canFit(-1), std::invalid_argument);
}
