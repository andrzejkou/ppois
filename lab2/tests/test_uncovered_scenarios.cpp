#include <gtest/gtest.h>

#include "AccountingDepartment.h"
#include "Classroom.h"
#include "Company.h"
#include "Curator.h"
#include "Day.h"
#include "DeanOffice.h"
#include "FinancialReport.h"
#include "Group.h"
#include "exceptions.h"
#include "Lecture.h"
#include "Mark.h"
#include "PaymentRecord.h"
#include "Premy.h"
#include "ScholarshipApplication.h"
#include "Student.h"
#include "StudentApplication.h"
#include "Subject.h"
#include "Submission.h"
#include "Teacher.h"
#include "TransferOrder.h"
#include "Worker.h"

#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

TEST(DeanOfficeTest, ConstLookupsReturnRegisteredObjectsAndNullForUnknownIds) {
  DeanOffice office("Engineering");
  const auto worker = std::make_shared<Worker>("Anna Smith", 35, "W1", "Secretary");
  office.addWorker(worker);
  office.addGroup(Group("G1", "CS", 2));
  ASSERT_TRUE(office.enrollStudent("G1", Student("Ivan Petrov", 20, "S1", 1)));

  const DeanOffice &view = office;
  ASSERT_EQ(view.workers().size(), 1U);
  EXPECT_EQ(view.workers().front(), worker);
  EXPECT_EQ(view.findWorker("W1"), worker.get());
  EXPECT_EQ(view.findWorker("unknown"), nullptr);
  EXPECT_EQ(view.findStudent("S1"), office.findStudent("S1"));
  EXPECT_EQ(view.findStudent("unknown"), nullptr);
  EXPECT_EQ(office.findStudent("unknown"), nullptr);
  EXPECT_FALSE(office.removeFromGroup("unknown", "S1"));
  EXPECT_EQ(office.groupOfStudent("S1"), "G1");
}

TEST(DeanOfficeTest, FullTargetKeepsStudentInSourceGroup) {
  DeanOffice office("Engineering");
  office.addGroup(Group("G1", "CS", 2));
  office.addGroup(Group("G2", "CS", 1));
  ASSERT_TRUE(office.enrollStudent("G1", Student("Ivan Petrov", 20, "S1", 1)));
  ASSERT_TRUE(office.enrollStudent("G2", Student("Anna Smith", 20, "S2", 1)));

  EXPECT_FALSE(office.groupHasCapacity("unknown"));
  EXPECT_FALSE(office.groupHasCapacity("G2"));
  EXPECT_FALSE(office.transferStudent("G1", "G2", "S1"));
  EXPECT_FALSE(office.mergeGroups("G1", "G2"));
  EXPECT_FALSE(office.mergeGroups("G1", "unknown"));
  EXPECT_EQ(office.groupOfStudent("S1"), "G1");
  EXPECT_EQ(office.groupOfStudent("S2"), "G2");
}

TEST(DeanOfficeTest, TransferRequiresMembershipInTheSpecifiedSource) {
  DeanOffice office("Engineering");
  office.addGroup(Group("G1", "CS", 2));
  office.addGroup(Group("G2", "CS", 2));
  office.addGroup(Group("G3", "CS", 2));
  ASSERT_TRUE(office.enrollStudent("G1", Student("Ivan Petrov", 20, "S1", 1)));
  ASSERT_TRUE(office.enrollStudent("G3", Student("Anna Smith", 20, "S2", 1)));

  const auto *student = office.findStudent("S2");
  ASSERT_NE(student, nullptr);
  EXPECT_FALSE(office.transferStudent("G1", "G2", "S2"));
  EXPECT_EQ(office.groupOfStudent("S1"), "G1");
  EXPECT_EQ(office.groupOfStudent("S2"), "G3");
  EXPECT_EQ(office.findStudent("S2"), student);
  ASSERT_NE(office.findGroup("G2"), nullptr);
  EXPECT_EQ(office.findGroup("G2")->size(), 0U);
}

TEST(DeanOfficeTest, DuplicateInPrefilledGroupsPreventsTransferAndMerge) {
  const Student duplicate("Ivan Petrov", 20, "S1", 1);
  Group source("G1", "CS", 3);
  source += Student("Anna Smith", 20, "S2", 1);
  source += duplicate;
  Group target("G2", "CS", 3);
  target += duplicate;

  DeanOffice office("Engineering");
  office.addGroup(source);
  office.addGroup(target);

  EXPECT_FALSE(office.transferStudent("G1", "G2", "S1"));
  EXPECT_FALSE(office.mergeGroups("G1", "G2"));
  ASSERT_NE(office.findGroup("G1"), nullptr);
  ASSERT_NE(office.findGroup("G2"), nullptr);
  EXPECT_EQ(office.findGroup("G1")->size(), 2U);
  EXPECT_EQ(office.findGroup("G2")->size(), 1U);
  EXPECT_FALSE(office.findGroup("G2")->containsStudent("S2"));
}

TEST(DeanOfficeTest, PromotionIncludesThresholdAndSkipsLowerAverage) {
  DeanOffice office("Engineering");
  office.addGroup(Group("G1", "CS", 2));
  const Subject subject("CPP", "Programming", 4);
  Student below("Ivan Petrov", 20, "S1", 1);
  Student threshold("Anna Smith", 20, "S2", 1);
  below.addMark(Mark(7, subject, Day("Mon", "2026-10-05")));
  threshold.addMark(Mark(8, subject, Day("Mon", "2026-10-05")));
  ASSERT_TRUE(office.enrollStudent("G1", below));
  ASSERT_TRUE(office.enrollStudent("G1", threshold));

  EXPECT_EQ(office.promoteEligibleStudents("G1", 8), 1U);
  EXPECT_EQ(office.findStudent("S1")->year(), 1);
  EXPECT_EQ(office.findStudent("S2")->year(), 2);
}

TEST(GroupTest, FullGroupRejectsLvalueStudentWithoutMutatingMembership) {
  Group group("G1", "CS", 1);
  const Student first("Ivan Petrov", 20, "S1", 1);
  const Student second("Anna Smith", 20, "S2", 1);
  group += first;

  EXPECT_THROW(group += second, GroupCapacityException);
  EXPECT_EQ(group.size(), 1U);
  EXPECT_TRUE(group.containsStudent("S1"));
  EXPECT_FALSE(group.containsStudent("S2"));
}

TEST(CuratorTest, RejectsNullGroupAndKeepsCurrentAssignment) {
  const std::shared_ptr<Group> missing;
  EXPECT_THROW(Curator("Anna Smith", 35, "W1", missing), std::invalid_argument);

  const auto group = std::make_shared<Group>("G1", "CS", 2);
  Curator curator("Anna Smith", 35, "W1", group);
  EXPECT_THROW(curator.assignGroup(missing), std::invalid_argument);
  EXPECT_EQ(&curator.groupCur(), group.get());
}

TEST(StudentTest, AcceptsDistinctWorksAndApplications) {
  Student student("Ivan Petrov", 20, "S1", 1);
  student.addSubmission(Submission("S1", "Lab 1"));
  student.addSubmission(Submission("S1", "Lab 2"));
  EXPECT_EQ(student.submissionCount(), 2U);
  EXPECT_TRUE(student.recordSubmissionScore("Lab 2", 8, 10));
  EXPECT_FALSE(student.submissions().front().graded());
  EXPECT_EQ(student.submissions().back().score(), 8);

  student.submitApplication(StudentApplication("S1", "Transfer"));
  student.submitApplication(StudentApplication("S1", "Leave"));
  EXPECT_EQ(student.applicationCount(), 2U);
  EXPECT_TRUE(student.reviewApplication("Leave"));
  EXPECT_TRUE(student.reviewApplication("Transfer"));

  student.requestScholarship(ScholarshipApplication("S1", "Academic"));
  student.requestScholarship(ScholarshipApplication("S1", "Social"));
  EXPECT_EQ(student.scholarshipApplicationCount(), 2U);
  EXPECT_TRUE(student.resolveScholarshipApplication("Social", true, ""));
  EXPECT_TRUE(student.resolveScholarshipApplication("Academic", false, "Insufficient results"));
}

TEST(OutputTest, ListsPartnersCoursesAndScholarshipEligibility) {
  Company company("Internship", "University");
  company.addPartner(std::make_shared<Company>("Training", "Partner"));
  Teacher teacher("Anna Smith", 35, "W1", "Engineering");
  teacher.assignCourse("Programming");
  Student student("Ivan Petrov", 20, "S1", 1);

  testing::internal::CaptureStdout();
  company.printInfo();
  teacher.printInfo();
  student.printInfo();
  const std::string output = testing::internal::GetCapturedStdout();

  EXPECT_NE(output.find("Partners:\n - Partner\n"), std::string::npos);
  EXPECT_NE(output.find("Courses:\n - Programming\n"), std::string::npos);
  EXPECT_NE(output.find("Eligible for scholarship: Yes\n"), std::string::npos);
}

TEST(AcademicBoundaryTest, LessonCanEndAtMidnightAndRequiresPositiveDuration) {
  const Subject subject("CPP", "Programming", 4);
  const Classroom room("101", 30);
  const Lecture lecture(subject, "W1", "Mon", 23, 1, room);

  EXPECT_EQ(lecture.durationHours(), 1);
  EXPECT_EQ(lecture.subject().credits(), 4);
  EXPECT_THROW(Lecture(subject, "W1", "Mon", 23, 0, room), std::invalid_argument);
}

TEST(AcademicBoundaryTest, TransferOrderRequiresDestination) {
  EXPECT_THROW(TransferOrder(1, "S1", "G1", ""), std::invalid_argument);
}

TEST(FinancialBoundaryTest, RejectsInvalidAmountsWithoutChangingSavedTotals) {
  FinancialReport report("October");
  report.addIncome(100);
  report.addExpense(20);
  Premy bonus("Annual", 50);

  for (const double amount : {-1.0, std::numeric_limits<double>::infinity(),
                              std::numeric_limits<double>::quiet_NaN()}) {
    SCOPED_TRACE(amount);
    EXPECT_THROW(report.addIncome(amount), std::invalid_argument);
    EXPECT_THROW(report.addExpense(amount), std::invalid_argument);
    EXPECT_THROW(Premy("Annual", amount), std::invalid_argument);
    EXPECT_THROW(bonus.changeAmount(amount), std::invalid_argument);
    EXPECT_THROW(PaymentRecord("S1", amount, "Tuition", "2026-10-31"),
                 std::invalid_argument);
  }

  EXPECT_DOUBLE_EQ(report.balance(), 80);
  EXPECT_DOUBLE_EQ(bonus.amount(), 50);
}

TEST(AccountingDepartmentTest, FiltersPaidRecordsByOctoberAndExactDate) {
  AccountingDepartment department("Engineering");
  PaymentRecord october("S1", 40, "Tuition", "2026-10-31");
  PaymentRecord november("S1", 20, "Tuition", "2026-11-01");
  PaymentRecord noDay("S1", 10, "Tuition", "2026-10");
  PaymentRecord unpaid("S2", 70, "Tuition", "2026-10-31");
  october.markPaid();
  november.markPaid();
  noDay.markPaid();
  department.recordPayment(october);
  department.recordPayment(november);
  department.recordPayment(noDay);
  department.recordPayment(unpaid);

  EXPECT_DOUBLE_EQ(department.makeReport("OCTOBER").balance(), -50);
  EXPECT_DOUBLE_EQ(department.makeReport("September").balance(), 0);
  EXPECT_DOUBLE_EQ(department.makeReport("2026-10-31").balance(), -40);
  EXPECT_DOUBLE_EQ(department.makeReport("2026-11-01").balance(), -20);
}

TEST(AccountingDepartmentTest, UsesUnfilteredTotalsForNonCalendarReportLabels) {
  AccountingDepartment department("Engineering");
  PaymentRecord payment("S1", 40, "Tuition", "2026-10-31");
  payment.markPaid();
  department.recordPayment(payment);
  department.recordPayment(PaymentRecord("S2", 70, "Tuition", "2026-10-31"));

  // A label outside the supported calendar formats produces a general report.
  for (const std::string label : {"General", "2026/10", "20x6-10", "2026-x1",
                                  "2026-1x", "2026-00", "2026-13",
                                  "2026-10/31", "2026-10-x1", "2026-10-3x",
                                  "2026-10-00", "2026-10-32"}) {
    SCOPED_TRACE(label);
    const auto report = department.makeReport(label);
    EXPECT_EQ(report.period(), label);
    EXPECT_DOUBLE_EQ(report.balance(), -40);
  }
}
