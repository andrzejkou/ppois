#include <gtest/gtest.h>

#include "AccountingDepartment.h"
#include "Curator.h"
#include "DeanOffice.h"
#include "Group.h"
#include "Mark.h"
#include "PaymentRecord.h"
#include "Student.h"
#include "Subject.h"
#include "Teacher.h"
#include "Day.h"
#include "Worker.h"
#include "exceptions.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace {
Subject covSubject(const std::string &code) {
  return Subject(code, code + " subject", 3);
}
Day covDay() { return Day("Monday", "2026-11-02"); }
}

TEST(RemainingBranchesStudent, MarkQueriesAndRemovalAlternatives) {
  Student student("Coverage", 20, "COV-ST", 2);
  EXPECT_FALSE(student.hasMarkForSubject("MATH"));
  EXPECT_DOUBLE_EQ(student.averageForSubject("MATH"), 0.0);
  EXPECT_EQ(student.removeMarksForSubject("MATH"), 0U);

  student.addMark(Mark(4, covSubject("MATH"), covDay()));
  student.addMark(Mark(8, covSubject("MATH"), covDay()));
  student.addMark(Mark(9, covSubject("PHYS"), covDay()));
  EXPECT_TRUE(student.hasMarkForSubject("MATH"));
  EXPECT_FALSE(student.hasMarkForSubject("CHEM"));
  EXPECT_DOUBLE_EQ(student.averageForSubject("MATH"), 6.0);
  EXPECT_DOUBLE_EQ(student.averageForSubject("CHEM"), 0.0);
  EXPECT_EQ(student.countMarksBelow(5), 1U);
  EXPECT_EQ(student.countMarksBelow(10), 3U);
  EXPECT_EQ(student.removeMarksForSubject("MATH"), 2U);
  EXPECT_EQ(student.removeMarksForSubject("MATH"), 0U);
  EXPECT_DOUBLE_EQ(student.averageMarkCheck(), 9.0);
  EXPECT_THROW(student.countMarksBelow(-1), std::invalid_argument);
  EXPECT_THROW(student.countMarksBelow(11), std::invalid_argument);
}

TEST(RemainingBranchesStudent, ScholarshipAndApplicationDecisionPaths) {
  Student student("Coverage", 20, "COV-APP", 2);
  EXPECT_FALSE(student.resolveScholarshipApplication("Unknown", true));
  EXPECT_THROW(student.requestScholarship(
                   ScholarshipApplication("OTHER", "Merit")),
               std::invalid_argument);
  student.requestScholarship(ScholarshipApplication("COV-APP", "Merit"));
  EXPECT_THROW(student.requestScholarship(
                   ScholarshipApplication("COV-APP", "Merit")),
               std::invalid_argument);
  EXPECT_TRUE(student.resolveScholarshipApplication("Merit", false, "Not eligible"));
  EXPECT_FALSE(student.resolveScholarshipApplication("Merit", true));

  EXPECT_THROW(student.submitApplication(StudentApplication("OTHER", "Leave")),
               std::invalid_argument);
  student.submitApplication(StudentApplication("COV-APP", "Leave"));
  EXPECT_THROW(student.submitApplication(StudentApplication("COV-APP", "Leave")),
               std::invalid_argument);
  EXPECT_FALSE(student.reviewApplication("Missing"));
  EXPECT_TRUE(student.reviewApplication("Leave"));
  EXPECT_FALSE(student.reviewApplication("Leave"));
}

TEST(RemainingBranchesGroup, SharedPointerAndCapacityAlternatives) {
  Group group("COV-G", "CS", 1);
  EXPECT_THROW(group += std::shared_ptr<Student>{}, std::invalid_argument);
  auto first = std::make_shared<Student>("First", 20, "COV-1", 2);
  group += first;
  EXPECT_THROW(group += first, DuplicateStudentException);
  EXPECT_THROW(group += std::make_shared<Student>("Second", 20, "COV-2", 2),
               GroupCapacityException);
  EXPECT_FALSE(group.hasCapacity());
  EXPECT_EQ(group.availableSeats(), 0U);
  EXPECT_EQ(group.findStudent("COV-1"), first);
  EXPECT_EQ(group.findStudent("missing"), nullptr);
  EXPECT_THROW(group.studentsInSubGroup(-1), std::invalid_argument);
  EXPECT_TRUE(group.containsStudent("COV-1"));
  EXPECT_FALSE(group.containsStudent("missing"));
  EXPECT_TRUE(group -= "COV-1");
  EXPECT_TRUE(group.hasCapacity());
}

TEST(RemainingBranchesCurator, EmptyGroupAndSubgroupAlternatives) {
  Group empty("COV-EMPTY", "CS", 5);
  Curator curator("Curator", 40, "COV-W", empty);
  EXPECT_DOUBLE_EQ(curator.groupAverageMark(), 0.0);
  EXPECT_EQ(curator.assignUnassignedStudentsToSubGroup(2), 0U);
  EXPECT_EQ(curator.reassignStudentsWithDebts(2), 0U);
  EXPECT_FALSE(curator.transferToSubGroup("missing", 2));
  EXPECT_FALSE(curator.removeStudent("missing"));
  EXPECT_THROW(curator.assignUnassignedStudentsToSubGroup(-1), std::invalid_argument);
  EXPECT_THROW(curator.reassignStudentsWithDebts(0), std::invalid_argument);

  Group group("COV-CUR", "CS", 4);
  Student debt("Debt", 20, "COV-D", 2);
  debt.setAcademicDebtCount(1);
  debt.setSubGroup(2);
  Student clean("Clean", 20, "COV-C", 2);
  group += debt;
  group += clean;
  curator.assignGroup(group);
  EXPECT_EQ(curator.reassignStudentsWithDebts(2), 0U);
  EXPECT_EQ(curator.reassignStudentsWithDebts(3), 1U);
  EXPECT_EQ(curator.assignUnassignedStudentsToSubGroup(1), 1U);
  EXPECT_EQ(curator.countSubGroup(3), 1U);
  EXPECT_EQ(curator.countSubGroup(1), 1U);
  EXPECT_EQ(curator.studentCount(), 2U);
  EXPECT_DOUBLE_EQ(curator.groupAverageMark(), 0.0);
}

TEST(RemainingBranchesTeacher, CourseReplacementAndSubjectLifecycle) {
  Teacher teacher("Teacher", 40, "COV-T", "Math");
  EXPECT_FALSE(teacher.replaceCourse("Absent", "New"));
  teacher.assignCourse("Algebra");
  EXPECT_FALSE(teacher.replaceCourse("Algebra", ""));
  EXPECT_FALSE(teacher.replaceCourse("Algebra", "Algebra"));
  teacher.assignCourse("Geometry");
  EXPECT_FALSE(teacher.replaceCourse("Algebra", "Geometry"));
  EXPECT_TRUE(teacher.replaceCourse("Algebra", "Calculus"));
  EXPECT_FALSE(teacher.replaceCourse("Absent", "New"));
  EXPECT_THROW(teacher.assignCourse(""), std::invalid_argument);
  EXPECT_THROW(teacher.assignCourse("Calculus"), std::invalid_argument);
  EXPECT_TRUE(teacher.removeCourse("Calculus"));
  EXPECT_FALSE(teacher.removeCourse("Calculus"));

  teacher.assignSubject(covSubject("MATH"));
  EXPECT_TRUE(teacher.teachesSubject("MATH"));
  EXPECT_FALSE(teacher.teachesSubject("PHYS"));
  EXPECT_THROW(teacher.assignSubject(covSubject("MATH")), std::invalid_argument);
  EXPECT_FALSE(teacher.removeSubject("PHYS"));
  EXPECT_TRUE(teacher.removeSubject("MATH"));
  teacher.clearTeachingAssignments();
  EXPECT_TRUE(teacher.subjects().empty());
  EXPECT_TRUE(teacher.courses().empty());
}

TEST(RemainingBranchesAccounting, MixedStudentPurposeAndPaidFilters) {
  AccountingDepartment accounting("Coverage accounting");
  Student student("Student", 20, "COV-PAY", 2);
  accounting.recordPayment(PaymentRecord("COV-PAY", 100, "Tuition", "2026-11-01"));
  accounting.recordPayment(PaymentRecord("COV-PAY", 50, "Books", "2026-11-02"));
  accounting.recordPayment(PaymentRecord("OTHER", 75, "Tuition", "2026-11-03"));
  EXPECT_DOUBLE_EQ(accounting.totalPaidForStudent("COV-PAY"), 0.0);
  EXPECT_DOUBLE_EQ(accounting.totalPaidByStudent(student), 0.0);
  EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Tuition", true), 0.0);
  EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Tuition", false), 175.0);
  EXPECT_EQ(accounting.paymentsForStudent("COV-PAY").size(), 2U);
  EXPECT_TRUE(accounting.hasPayment("COV-PAY", "Tuition"));
  EXPECT_FALSE(accounting.hasPayment("COV-PAY", "Dorm"));
  EXPECT_EQ(accounting.markStudentPaymentsPaid("OTHER"), 1U);
  EXPECT_EQ(accounting.settleUnpaidPaymentsForPurpose("COV-PAY", "Books"), 1U);
  EXPECT_EQ(accounting.removeUnpaidPayments("COV-PAY"), 1U);
  EXPECT_EQ(accounting.cancelUnpaidPaymentsForPurpose("COV-PAY", "Tuition"), 0U);
  EXPECT_DOUBLE_EQ(accounting.totalPaidForStudent("COV-PAY"), 50.0);
  EXPECT_DOUBLE_EQ(accounting.totalPaidByStudent(student), 50.0);
}

TEST(RemainingBranchesDeanOffice, WorkerScholarshipTransferMergeAndPromotion) {
  DeanOffice office("Coverage office");
  Group source("COV-SRC", "CS", 3);
  Group target("COV-DST", "CS", 3);
  Group otherSpecialty("COV-OTHER", "MATH", 3);
  office.addGroup(source);
  office.addGroup(target);
  office.addGroup(otherSpecialty);

  EXPECT_FALSE(office.transferStudent("missing", "COV-DST", "none"));
  EXPECT_FALSE(office.transferStudent("COV-SRC", "COV-SRC", "none"));
  EXPECT_FALSE(office.mergeGroups("COV-SRC", "COV-SRC"));
  EXPECT_FALSE(office.mergeGroups("missing", "COV-DST"));
  EXPECT_FALSE(office.mergeGroups("COV-SRC", "COV-OTHER"));

  Student good("Good", 20, "COV-GOOD", 1);
  good.addMark(Mark(10, covSubject("MATH"), covDay()));
  Student debt("Debt", 20, "COV-DEBT", 1);
  debt.setAcademicDebtCount(1);
  EXPECT_TRUE(office.enrollStudent("COV-SRC", good));
  EXPECT_TRUE(office.enrollStudent("COV-SRC", debt));
  EXPECT_EQ(office.promoteEligibleStudents("missing", 5.0), 0U);
  EXPECT_EQ(office.promoteEligibleStudents("COV-SRC", 9.0), 1U);
  EXPECT_EQ(office.findStudent("COV-GOOD")->year(), 2);
  EXPECT_EQ(office.studentsWithAcademicDebts("COV-SRC"), 1U);
  EXPECT_THROW(office.promoteEligibleStudents("COV-SRC", 11.0), std::invalid_argument);
  EXPECT_THROW(office.giveScholarship("missing"), std::invalid_argument);
  EXPECT_THROW(office.giveScholarship("COV-DEBT"), std::invalid_argument);
  office.giveScholarship("COV-GOOD");

  auto teacher = std::make_shared<Teacher>("Teacher", 40, "COV-TEACH", "CS");
  office.addWorker(teacher);
  EXPECT_FALSE(office.assignCuratorToGroup("COV-TEACH", "COV-SRC"));
  EXPECT_FALSE(office.assignCuratorToGroup("missing", "COV-SRC"));
  EXPECT_FALSE(office.assignCuratorToGroup("COV-TEACH", "missing"));
  auto curator = std::make_shared<Curator>("Curator", 40, "COV-CURATOR", Group("COV-CG", "CS", 2));
  office.addWorker(curator);
  EXPECT_TRUE(office.assignCuratorToGroup("COV-CURATOR", "COV-DST"));
  EXPECT_THROW(office.addWorker(nullptr), std::invalid_argument);
  EXPECT_THROW(office.addWorker(teacher), std::invalid_argument);
  EXPECT_FALSE(office.removeWorker("missing"));
  EXPECT_TRUE(office.removeWorker("COV-TEACH"));

  EXPECT_TRUE(office.mergeGroups("COV-SRC", "COV-DST"));
  EXPECT_EQ(office.findGroup("COV-SRC")->size(), 0U);
  EXPECT_EQ(office.findGroup("COV-DST")->size(), 2U);
  EXPECT_TRUE(office.mergeGroups("COV-SRC", "COV-DST")); // Empty source is a valid no-op merge.
}
