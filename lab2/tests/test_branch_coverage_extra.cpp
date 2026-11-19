#include <gtest/gtest.h>

#include "AccountingDepartment.h"
#include "Benefit.h"
#include "Company.h"
#include "Contract.h"
#include "Curator.h"
#include "Day.h"
#include "DeanOffice.h"
#include "Exam.h"
#include "exceptions.h"
#include "Group.h"
#include "Lesson.h"
#include "Lecture.h"
#include "Worker.h"
#include "Mark.h"
#include "PaymentRecord.h"
#include "People.h"
#include "ScholarshipApplication.h"
#include "Student.h"
#include "Submission.h"
#include "Subject.h"
#include "Teacher.h"
#include "Week.h"
#include "Classroom.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

Subject coverageMath() {
  return Subject("MATH", "Mathematics", 5);
}

Classroom coverageRoom() {
  return Classroom("COV-101", 30);
}

std::shared_ptr<Lecture> coverageLecture(const std::string &teacher,
                                         int start = 9,
                                         int duration = 2) {
  return std::make_shared<Lecture>(coverageMath(), teacher, "Monday",
                                   start, duration, coverageRoom());
}

} // namespace

TEST(BranchCoverage, StudentEmptyCollectionsAndMoveConstruction) {
  Student empty("Empty Student", 19, "EMPTY-1", 1);
  empty.averageMarkMake();
  EXPECT_DOUBLE_EQ(empty.averageMarkCheck(), 0.0);
  EXPECT_FALSE(empty.hasMarkForSubject("MATH"));
  EXPECT_DOUBLE_EQ(empty.averageForSubject("MATH"), 0.0);
  EXPECT_EQ(empty.removeMarksForSubject("MATH"), 0U);
  EXPECT_EQ(empty.countMarksBelow(0), 0U);
  EXPECT_EQ(empty.countMarksBelow(10), 0U);
  EXPECT_EQ(empty.recalculateAcademicDebtCount(0), 0U);
  EXPECT_EQ(empty.recalculateAcademicDebtCount(10), 0U);
  empty.setSubGroup(0);
  EXPECT_EQ(empty.subGroupNum(), 0);

  Student source("Move Student", 20, "MOVE-1", 2);
  source.addMark(Mark(4, coverageMath(), Day("Monday", "2026-11-02")));
  source.setSubGroup(2);
  Student moved(std::move(source));
  EXPECT_EQ(moved.studentId(), "MOVE-1");
  EXPECT_EQ(moved.subGroupNum(), 2);
  EXPECT_DOUBLE_EQ(moved.averageMarkCheck(), 4.0);
}

TEST(BranchCoverage, StudentSubmissionAndApplicationNegativePaths) {
  Student student("Student", 20, "ST-1", 2);
  Submission graded("ST-1", "Graded");
  graded.recordScore(1, 1);
  EXPECT_THROW(student.addSubmission(graded), std::invalid_argument);
  student.addSubmission(Submission("ST-1", "Lab"));
  EXPECT_THROW(student.addSubmission(Submission("ST-1", "Lab")),
               std::invalid_argument);
  EXPECT_THROW(student.addSubmission(Submission("OTHER", "Other")),
               std::invalid_argument);
  EXPECT_THROW(student.recordSubmissionScore("Lab", 11, 10),
               std::out_of_range);
  EXPECT_THROW(student.recordSubmissionScore("Lab", 1, 0),
               std::invalid_argument);
  EXPECT_FALSE(student.recordSubmissionScore("Missing", 5, 10));

  EXPECT_THROW(student.submitApplication(StudentApplication("OTHER", "Topic")),
               std::invalid_argument);
  student.submitApplication(StudentApplication("ST-1", "Topic"));
  EXPECT_THROW(student.submitApplication(StudentApplication("ST-1", "Topic")),
               std::invalid_argument);
  EXPECT_FALSE(student.reviewApplication("Missing"));

  EXPECT_THROW(student.requestScholarship(
                   ScholarshipApplication("OTHER", "Merit")),
               std::invalid_argument);
  student.requestScholarship(ScholarshipApplication("ST-1", "Merit"));
  EXPECT_THROW(student.resolveScholarshipApplication("Merit", false),
               std::invalid_argument);
  EXPECT_TRUE(student.resolveScholarshipApplication("Merit", false, "Low marks"));
  EXPECT_FALSE(student.resolveScholarshipApplication("Merit", true));
}

TEST(BranchCoverage, StudentDebtAndScholarshipBoundaries) {
  Student student("Student", 20, "ST-2", 2);
  Subject math = coverageMath();
  Subject physics("PHYS", "Physics", 4);
  Day day("Monday", "2026-11-02");
  student.addMark(Mark(4, math, day));
  student.addMark(Mark(3, math, day));
  student.addMark(Mark(9, physics, day));

  EXPECT_EQ(student.recalculateAcademicDebtCount(5), 1U);
  EXPECT_EQ(student.academicDebtCount(), 1);
  EXPECT_EQ(student.recalculateAcademicDebtCount(3), 0U);
  EXPECT_EQ(student.academicDebtCount(), 0);
  EXPECT_EQ(student.countMarksBelow(5), 2U);
  EXPECT_EQ(student.removeMarksForSubject("UNKNOWN"), 0U);
  EXPECT_EQ(student.removeMarksForSubject("MATH"), 2U);
  EXPECT_DOUBLE_EQ(student.averageMarkCheck(), 9.0);
  student.awardScholarship();
  student.setAcademicDebtCount(0);
  EXPECT_TRUE(student.isEligibleForScholarship());
  student.setAcademicDebtCount(-1);
  EXPECT_EQ(student.academicDebtCount(), 0);
}

TEST(BranchCoverage, GroupAllInsertionAndSubgroupPaths) {
  Group group("BR-G1", "CS", 2);
  Student first("Alice", 20, "BR-S1", 2);
  Student second("Bob", 20, "BR-S2", 2);

  EXPECT_FALSE(group["missing"]);
  EXPECT_TRUE(group.hasCapacity());
  EXPECT_EQ(group.findStudent("missing"), nullptr);
  EXPECT_FALSE(group.changeStudentSubGroup("missing", 1));
  group += first;
  EXPECT_THROW(group.changeStudentSubGroup("BR-S1", -1), std::invalid_argument);
  EXPECT_THROW(group += first, DuplicateStudentException);
  auto secondPtr = std::make_shared<Student>(second);
  group += secondPtr;
  EXPECT_THROW(group += secondPtr, DuplicateStudentException);
  EXPECT_FALSE(group.hasCapacity());
  EXPECT_EQ(group.availableSeats(), 0U);

  EXPECT_THROW(group += Student("Third", 20, "BR-S3", 2),
               GroupCapacityException);
  EXPECT_THROW(group += std::make_shared<Student>(
                   "Third", 20, "BR-S3", 2),
               GroupCapacityException);
  EXPECT_THROW(group += std::shared_ptr<Student>(), std::invalid_argument);

  Group empty("BR-EMPTY", "CS", 1);
  EXPECT_EQ(empty.assignSubGroup(2), 0U);
  EXPECT_EQ(empty.studentsInSubGroup(2).size(), 0U);
  EXPECT_THROW(empty.studentsInSubGroup(-1), std::invalid_argument);
  EXPECT_FALSE(empty -= "missing");
  EXPECT_EQ(empty.unassignedStudentCount(), 0U);
}

TEST(BranchCoverage, GroupSortingAndSubgroupSelection) {
  Group group("BR-G2", "CS", 4);
  Student zed("Zed", 20, "Z", 2);
  Student amy("Amy", 20, "A", 2);
  Student max("Max", 20, "M", 2);
  zed.addMark(Mark(5, coverageMath(), Day("Monday", "2026-11-02")));
  amy.addMark(Mark(9, coverageMath(), Day("Monday", "2026-11-02")));
  max.addMark(Mark(9, coverageMath(), Day("Monday", "2026-11-02")));
  group += zed;
  group += amy;
  group += max;
  EXPECT_EQ(group.assignSubGroup(1), 3U);
  EXPECT_EQ(group.studentsInSubGroup(9).size(), 0U);
  EXPECT_TRUE(group.changeStudentSubGroup("A", 2));
  EXPECT_FALSE(group.changeStudentSubGroup("not-there", 2));
  EXPECT_EQ(group.studentsInSubGroup(2).size(), 1U);
  group.sortByName();
  EXPECT_EQ(group.students().front()->fullName(), "Amy");
  group.sortByAverageMark();
  EXPECT_EQ(group.students().front()->averageMarkCheck(), 9.0);
}

TEST(BranchCoverage, AccountingFiltersAndNoMatchPaths) {
  EXPECT_THROW(AccountingDepartment(""), std::invalid_argument);
  AccountingDepartment accounting("Coverage accounting");
  Student first("First", 20, "ACC-1", 2);
  Student second("Second", 20, "ACC-2", 2);

  EXPECT_DOUBLE_EQ(accounting.totalPaid(), 0.0);
  EXPECT_DOUBLE_EQ(accounting.totalUnpaid(), 0.0);
  EXPECT_DOUBLE_EQ(accounting.totalPaidForStudent("ACC-1"), 0.0);
  EXPECT_DOUBLE_EQ(accounting.totalPaidByStudent(first), 0.0);
  EXPECT_EQ(accounting.paymentCountForStudent(first), 0U);
  EXPECT_FALSE(accounting.hasPayment("ACC-1", "Tuition"));
  EXPECT_TRUE(accounting.paymentsForStudent("ACC-1").empty());
  EXPECT_EQ(accounting.markStudentPaymentsPaid("ACC-1"), 0U);
  EXPECT_EQ(accounting.removeUnpaidPayments("ACC-1"), 0U);
  EXPECT_EQ(accounting.cancelUnpaidPaymentsForPurpose("ACC-1", "Tuition"), 0U);
  EXPECT_EQ(accounting.settleUnpaidPaymentsForPurpose("ACC-1", "Tuition"), 0U);
  EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Tuition", true), 0.0);
  EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Tuition", false), 0.0);

  accounting.recordPayment(PaymentRecord("ACC-1", 100, "Tuition", "2026-11-01"));
  accounting.recordPayment(PaymentRecord("ACC-2", 50, "Books", "2026-11-02"));
  EXPECT_EQ(accounting.markStudentPaymentsPaid("ACC-2"), 1U);
  EXPECT_EQ(accounting.markStudentPaymentsPaid("ACC-2"), 0U);
  EXPECT_EQ(accounting.settleUnpaidPaymentsForPurpose("ACC-1", "Other"), 0U);
  EXPECT_EQ(accounting.cancelUnpaidPaymentsForPurpose("ACC-2", "Books"), 0U);
  EXPECT_EQ(accounting.removeUnpaidPayments("ACC-2"), 0U);
  EXPECT_DOUBLE_EQ(accounting.totalPaidForStudent("ACC-2"), 50.0);
  EXPECT_DOUBLE_EQ(accounting.totalPaidByStudent(second), 50.0);
  EXPECT_EQ(accounting.paymentCountForStudent(first), 1U);
  EXPECT_EQ(accounting.paymentsForStudent("missing").size(), 0U);
  EXPECT_DOUBLE_EQ(accounting.makeReport("Empty").balance(), -50.0);
}

TEST(BranchCoverage, PaymentConstructorValidation) {
  PaymentRecord payment("PAY-1", 100, "Tuition", "2026-11-01");
  EXPECT_THROW(PaymentRecord("S", 1, "", "date"), std::invalid_argument);
  EXPECT_THROW(PaymentRecord("S", 1, "Purpose", ""), std::invalid_argument);
  EXPECT_THROW(PaymentRecord("S", -1, "Purpose", "date"), std::invalid_argument);
}

TEST(BranchCoverage, TeacherCourseAndSubjectAlternativePaths) {
  EXPECT_THROW(Teacher("Teacher", 40, "T-EMPTY", ""), std::invalid_argument);
  Teacher teacher("Teacher", 40, "T-COV", "Math");
  EXPECT_THROW(teacher.assignCourse(""), std::invalid_argument);
  teacher.assignCourse("Algebra");
  EXPECT_FALSE(teacher.replaceCourse("Algebra", ""));
  EXPECT_FALSE(teacher.replaceCourse("Algebra", "Algebra"));
  teacher.assignCourse("Geometry");
  EXPECT_FALSE(teacher.replaceCourse("Algebra", "Geometry"));
  EXPECT_FALSE(teacher.replaceCourse("Missing", "Calculus"));
  EXPECT_TRUE(teacher.replaceCourse("Algebra", "Calculus"));
  EXPECT_FALSE(teacher.removeCourse("Missing"));
  EXPECT_TRUE(teacher.removeCourse("Calculus"));

  Subject math = coverageMath();
  teacher.assignSubject(math);
  EXPECT_THROW(teacher.assignSubject(math), std::invalid_argument);
  EXPECT_TRUE(teacher.teachesSubject("MATH"));
  EXPECT_FALSE(teacher.teachesSubject("PHYS"));
  EXPECT_FALSE(teacher.removeSubject("PHYS"));
  EXPECT_TRUE(teacher.removeSubject("MATH"));
  teacher.clearTeachingAssignments();
  EXPECT_TRUE(teacher.courses().empty());
  EXPECT_TRUE(teacher.subjects().empty());
}

TEST(BranchCoverage, CuratorEmptyAndConditionalPaths) {
  Group emptyGroup("CUR-EMPTY", "CS", 3);
  Curator emptyCurator("Empty Curator", 40, "CUR-E", emptyGroup);
  EXPECT_DOUBLE_EQ(emptyCurator.groupAverageMark(), 0.0);
  EXPECT_EQ(emptyCurator.studentsWithDebts(), 0U);
  EXPECT_EQ(emptyCurator.countSubGroup(1), 0U);
  EXPECT_EQ(emptyCurator.assignUnassignedStudentsToSubGroup(1), 0U);
  EXPECT_EQ(emptyCurator.reassignStudentsWithDebts(1), 0U);
  EXPECT_FALSE(emptyCurator.removeStudent("missing"));
  EXPECT_FALSE(emptyCurator.transferToSubGroup("missing", 1));

  Group group("CUR-G", "CS", 3);
  Student noDebt("No debt", 20, "CUR-1", 2);
  Student debt("Debt", 20, "CUR-2", 2);
  noDebt.setSubGroup(1);
  debt.setAcademicDebtCount(1);
  debt.setSubGroup(2);
  group += noDebt;
  group += debt;
  Curator curator("Curator", 40, "CUR-C", group);
  EXPECT_EQ(curator.assignUnassignedStudentsToSubGroup(3), 0U);
  EXPECT_EQ(curator.reassignStudentsWithDebts(2), 0U);
  EXPECT_EQ(curator.reassignStudentsWithDebts(3), 1U);
  EXPECT_EQ(curator.studentsWithDebts(), 1U);
  EXPECT_EQ(curator.countSubGroup(3), 1U);
  EXPECT_TRUE(curator.supervises("CUR-1"));
  EXPECT_FALSE(curator.supervises("missing"));
}

TEST(BranchCoverage, DayWeekAndLessonBoundaries) {
  EXPECT_THROW(Day("", "2026-11-01"), std::invalid_argument);
  EXPECT_THROW(Day("Monday", ""), std::invalid_argument);
  Day day("Monday", "2026-11-02");
  EXPECT_THROW(day.addLesson(nullptr), std::invalid_argument);
  EXPECT_FALSE(day.hasTeacher("T"));
  EXPECT_FALSE(day.hasSubject("MATH"));
  EXPECT_FALSE(day.hasTimeConflict(8, 1));

  EXPECT_THROW(Lecture(coverageMath(), "", "Monday", 9, 1, coverageRoom()),
               std::invalid_argument);
  EXPECT_THROW(Lecture(coverageMath(), "T", "", 9, 1, coverageRoom()),
               std::invalid_argument);
  EXPECT_THROW(Lecture(coverageMath(), "T", "Monday", -1, 1, coverageRoom()),
               std::invalid_argument);
  EXPECT_THROW(Lecture(coverageMath(), "T", "Monday", 24, 1, coverageRoom()),
               std::invalid_argument);
  EXPECT_THROW(Lecture(coverageMath(), "T", "Monday", 23, 2, coverageRoom()),
               std::invalid_argument);

  day.addLesson(coverageLecture("T1", 9, 2));
  EXPECT_TRUE(day.hasTimeConflict(9, 1));
  EXPECT_TRUE(day.hasTimeConflict(10, 2));
  EXPECT_FALSE(day.hasTimeConflict(11, 1));
  EXPECT_THROW(day.hasTimeConflict(24, 1), std::invalid_argument);
  EXPECT_THROW(day.hasTimeConflict(1, 0), std::invalid_argument);
  EXPECT_EQ(day.lessonsByTeacher("T1").size(), 1U);
  EXPECT_TRUE(day.lessonsByTeacher("T2").empty());

  Week week(1);
  EXPECT_FALSE(week.hasTeacher("T1"));
  EXPECT_FALSE(week.hasTimeConflict("Monday", 9, 1));
  week.addDay(day);
  EXPECT_TRUE(week.hasTeacher("T1"));
  EXPECT_TRUE(week.hasTimeConflict("Monday", 9, 1));
  EXPECT_FALSE(week.hasTimeConflict("Tuesday", 9, 1));
  EXPECT_FALSE(week.removeDay("unknown"));
}

TEST(BranchCoverage, ScholarshipResolutionAndCompanyContracts) {
  ScholarshipApplication application("S", "Merit");
  EXPECT_THROW(application.reject(""), std::invalid_argument);
  application.approve();
  EXPECT_THROW(application.reject("Late"), std::logic_error);

  auto first = std::make_shared<Company>("Research", "First");
  auto second = std::make_shared<Company>("Education", "Second");
  EXPECT_THROW(first->addPartner(nullptr), std::invalid_argument);
  EXPECT_THROW(first->addPartner(first), std::invalid_argument);
  first->addPartner(second);
  EXPECT_TRUE(first->hasPartner("Second"));
  EXPECT_FALSE(first->hasPartner("Missing"));
  EXPECT_THROW(first->addPartner(second), std::invalid_argument);
  EXPECT_FALSE(first->removePartner("Missing"));
  EXPECT_TRUE(first->removePartner("Second"));

  Contract contract;
  EXPECT_THROW(contract.makeContract(), std::logic_error);
  contract.addCompany(*first);
  EXPECT_THROW(contract.addCompany(*first), std::invalid_argument);
  EXPECT_THROW(contract.makeContract(), std::logic_error);
  contract.addCompany(*second);
  contract.makeContract();
  EXPECT_TRUE(contract.isMade());
}

TEST(BranchCoverage, BenefitAndWorkerValidation) {
  Benefit benefit("Social", 100, "Support");
  EXPECT_FALSE(benefit.assignedTo("S1"));
  EXPECT_THROW(benefit.assignTo(""), std::invalid_argument);
  benefit.assignTo("S1");
  EXPECT_TRUE(benefit.assignedTo("S1"));
  EXPECT_THROW(benefit.assignTo("S1"), std::invalid_argument);
  EXPECT_FALSE(benefit.assignedTo("S2"));

  Worker worker("Worker", 35, "W-COV", "Secretary");
  EXPECT_EQ(worker.role(), "Сотрудник");
  EXPECT_THROW(worker.changePosition(""), std::invalid_argument);
  worker.changePosition("Senior secretary");
  EXPECT_EQ(worker.position(), "Senior secretary");
}
