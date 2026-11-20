#include <gtest/gtest.h>

#include "AccountingDepartment.h"
#include "Benefit.h"
#include "Classroom.h"
#include "Company.h"
#include "Curator.h"
#include "Day.h"
#include "DeanOffice.h"
#include "Group.h"
#include "Lecture.h"
#include "Mark.h"
#include "PaymentRecord.h"
#include "Schedule.h"
#include "ScholarshipApplication.h"
#include "Student.h"
#include "Subject.h"
#include "Teacher.h"
#include "Week.h"
#include "Worker.h"
#include "exceptions.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
Subject branchSubject(const std::string &code = "MATH") {
  return Subject(code, "Mathematics", 5);
}
Day branchDay() { return Day("Monday", "2026-11-02"); }
} // namespace

TEST(BranchLinesFollowup, StudentDebtDeduplicationAndScholarshipUpdates) {
  Student student("Branch Student", 20, "BR-ST-1", 2);
  const auto day = branchDay();
  student.addMark(Mark(2, branchSubject("MATH"), day));
  student.addMark(Mark(3, branchSubject("MATH"), day));
  student.addMark(Mark(4, branchSubject("PHYS"), day));
  EXPECT_EQ(student.recalculateAcademicDebtCount(5), 2U);
  EXPECT_EQ(student.recalculateAcademicDebtCount(3), 1U);
  EXPECT_THROW(student.recalculateAcademicDebtCount(-1), std::invalid_argument);
  EXPECT_THROW(student.recalculateAcademicDebtCount(11), std::invalid_argument);

  student.setAcademicDebtCount(0);
  student.awardScholarship();
  student.addMark(Mark(10, branchSubject("MATH"), day));
  student.averageMarkMake();
  student.awardScholarship();
  EXPECT_TRUE(student.isEligibleForScholarship());
  student.setAcademicDebtCount(1);
  EXPECT_FALSE(student.isEligibleForScholarship());
  student.setAcademicDebtCount(0);

  Benefit benefit("Social", 500, "Social support");
  EXPECT_FALSE(benefit.assignedTo("BR-ST-1"));
  benefit.assignTo("BR-ST-1");
  EXPECT_TRUE(benefit.assignedTo("BR-ST-1"));
  EXPECT_THROW(benefit.assignTo("BR-ST-1"), std::invalid_argument);
  EXPECT_THROW(benefit.assignTo(""), std::invalid_argument);
  student.assignBenefit(Benefit("Travel", 100, "Travel support"));
  EXPECT_THROW(student.assignBenefit(benefit), std::invalid_argument);
  student.removeBenefit();
  EXPECT_THROW(student.removeBenefit(), std::invalid_argument);
}

TEST(BranchLinesFollowup, StudentApplicationsResolveAndPrintInfo) {
  Student student("Print Student", 21, "BR-ST-2", 3);
  student.submitApplication(StudentApplication("BR-ST-2", "Leave"));
  EXPECT_TRUE(student.reviewApplication("Leave"));
  EXPECT_FALSE(student.reviewApplication("Leave"));
  EXPECT_FALSE(student.reviewApplication("Absent"));

  student.requestScholarship(ScholarshipApplication("BR-ST-2", "Merit"));
  EXPECT_TRUE(student.resolveScholarshipApplication("Merit", true));
  EXPECT_FALSE(student.resolveScholarshipApplication("Merit", true));
  student.setAcademicDebtCount(2);
  std::ostringstream output;
  auto *old = std::cout.rdbuf(output.rdbuf());
  student.printInfo();
  std::cout.rdbuf(old);
  EXPECT_NE(output.str().find("Academic debts"), std::string::npos);
}

TEST(BranchLinesFollowup, AccountingPaidUnpaidAndPurposeCombinations) {
  AccountingDepartment accounting("Branch accounting");
  accounting.recordPayment(PaymentRecord("A", 100, "Tuition", "2026-11-01"));
  accounting.recordPayment(PaymentRecord("A", 200, "Books", "2026-11-02"));
  accounting.recordPayment(PaymentRecord("B", 300, "Tuition", "2026-11-03"));
  EXPECT_EQ(accounting.markStudentPaymentsPaid("A"), 2U);
  EXPECT_EQ(accounting.markStudentPaymentsPaid("A"), 0U);
  EXPECT_DOUBLE_EQ(accounting.totalPaid(), 300.0);
  EXPECT_DOUBLE_EQ(accounting.totalUnpaid(), 300.0);
  EXPECT_DOUBLE_EQ(accounting.totalPaidForStudent("A"), 300.0);
  EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Tuition", true), 100.0);
  EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Tuition", false), 400.0);
  EXPECT_EQ(accounting.settleUnpaidPaymentsForPurpose("B", "Tuition"), 1U);
  EXPECT_EQ(accounting.settleUnpaidPaymentsForPurpose("B", "Tuition"), 0U);
  EXPECT_EQ(accounting.cancelUnpaidPaymentsForPurpose("A", "Books"), 0U);
  accounting.recordPayment(PaymentRecord("C", 50, "Dorm", "2026-11-04"));
  EXPECT_EQ(accounting.cancelUnpaidPaymentsForPurpose("C", "Dorm"), 1U);
  accounting.recordPayment(PaymentRecord("D", 60, "Other", "2026-11-05"));
  EXPECT_EQ(accounting.removeUnpaidPayments("D"), 1U);
  Student student("Account student", 20, "A", 2);
  EXPECT_EQ(accounting.paymentCountForStudent(student), 2U);
  EXPECT_DOUBLE_EQ(accounting.totalPaidByStudent(student), 300.0);
}

TEST(BranchLinesFollowup, CompanyPartnerAllBranchesAndOutput) {
  EXPECT_THROW(Company("", "Name"), std::invalid_argument);
  EXPECT_THROW(Company("Contract", ""), std::invalid_argument);
  Company mainCompany("Contract", "Main");
  auto partner = std::make_shared<Company>("Contract", "Partner");
  EXPECT_THROW(mainCompany.addPartner(nullptr), std::invalid_argument);
  EXPECT_THROW(mainCompany.addPartner(
                   std::shared_ptr<Company>(&mainCompany, [](Company *) {})),
               std::invalid_argument);
  mainCompany.addPartner(partner);
  EXPECT_TRUE(mainCompany.hasPartner("Partner"));
  EXPECT_THROW(mainCompany.addPartner(partner), std::invalid_argument);
  EXPECT_FALSE(mainCompany.removePartner("Missing"));
  EXPECT_TRUE(mainCompany.removePartner("Partner"));
  EXPECT_FALSE(mainCompany.hasPartner("Partner"));
  std::ostringstream output;
  auto *old = std::cout.rdbuf(output.rdbuf());
  mainCompany.printInfo();
  std::cout.rdbuf(old);
  EXPECT_NE(output.str().find("Partners"), std::string::npos);
}

TEST(BranchLinesFollowup, GroupRvalueInsertionAndSelectionBranches) {
  Group group("BR-GR", "CS", 4);
  group += Student("Zed", 20, "BR-Z", 2);
  group += Student("Amy", 20, "BR-A", 2);
  EXPECT_TRUE(group["BR-Z"]);
  EXPECT_FALSE(group["missing"]);
  EXPECT_THROW(group += Student("Zed2", 20, "BR-Z", 2),
               DuplicateStudentException);
  EXPECT_TRUE(group.changeStudentSubGroup("BR-Z", 1));
  EXPECT_FALSE(group.changeStudentSubGroup("missing", 1));
  EXPECT_EQ(group.studentsInSubGroup(1).size(), 1U);
  EXPECT_EQ(group.studentsInSubGroup(2).size(), 0U);
  EXPECT_EQ(group.assignSubGroup(2), 2U);
  EXPECT_EQ(group.unassignedStudentCount(), 0U);
  EXPECT_EQ(group.assignSubGroup(1), 2U);
  group.sortByName();
  EXPECT_EQ(group.students().front()->fullName(), "Amy");
  EXPECT_TRUE(group -= "BR-Z");
  EXPECT_FALSE(group -= "BR-Z");
}

TEST(BranchLinesFollowup, ScheduleWeekTeacherAndEmptyWeekBranches) {
  EXPECT_THROW(Schedule(""), std::invalid_argument);
  Schedule schedule("BR-SCHEDULE");
  Week emptyWeek(1);
  Week populatedWeek(2);
  Day monday("Monday", "2026-11-02");
  Classroom room("BR-101", 20);
  auto lecture =
      std::make_shared<Lecture>(branchSubject(), "T-BR", "Monday", 9, 2, room);
  monday.addLesson(lecture);
  populatedWeek.addDay(monday);
  schedule.addWeek(emptyWeek);
  schedule.addWeek(populatedWeek);
  EXPECT_THROW(schedule.addWeek(emptyWeek), std::invalid_argument);
  EXPECT_EQ(schedule.removeWeeksWithoutLessons(), 1U);
  EXPECT_TRUE(schedule.hasTeacher("T-BR"));
  EXPECT_FALSE(schedule.hasTeacher("missing"));
  EXPECT_TRUE(schedule.hasTimeConflict(2, "Monday", 10, 1));
  EXPECT_FALSE(schedule.hasTimeConflict(99, "Monday", 10, 1));
  EXPECT_FALSE(schedule.removeWeek(99));
  EXPECT_TRUE(schedule.removeWeek(2));
}

TEST(BranchLinesFollowup, CuratorAssignsAndReassignsOnlyMatchingStudents) {
  Group group("BR-CUR", "CS", 5);
  Student debt("Debt", 20, "BR-D", 2);
  debt.setAcademicDebtCount(2);
  Student clean("Clean", 20, "BR-C", 2);
  clean.setSubGroup(1);
  group += debt;
  group += clean;
  Curator curator("Curator", 40, "BR-W", group);
  EXPECT_EQ(curator.assignUnassignedStudentsToSubGroup(2), 1U);
  EXPECT_EQ(curator.assignUnassignedStudentsToSubGroup(2), 0U);
  EXPECT_EQ(curator.reassignStudentsWithDebts(3), 1U);
  EXPECT_EQ(curator.reassignStudentsWithDebts(3), 0U);
  EXPECT_THROW(curator.assignUnassignedStudentsToSubGroup(0),
               std::invalid_argument);
  EXPECT_THROW(curator.reassignStudentsWithDebts(0), std::invalid_argument);
  EXPECT_EQ(curator.studentsWithDebts(), 1U);
  EXPECT_EQ(curator.countSubGroup(3), 1U);
  EXPECT_TRUE(curator.supervises("BR-D"));
  EXPECT_FALSE(curator.supervises("missing"));
}

TEST(BranchLinesFollowup, DeanOfficeStudentGroupAndSpecialtyAlternatives) {
  DeanOffice office("Branch dean office");
  Group first("BR-D1", "CS", 3);
  Group second("BR-D2", "CS", 3);
  office.addGroup(first);
  office.addGroup(first);
  office.addGroup(second);
  EXPECT_EQ(office.groupCount(), 2U);
  Student student("Office student", 20, "BR-O", 2);
  EXPECT_TRUE(office.enrollStudent("BR-D1", student));
  EXPECT_FALSE(office.enrollStudent("BR-D1", student));
  EXPECT_FALSE(
      office.enrollStudent("missing", Student("Other", 20, "BR-X", 2)));
  EXPECT_EQ(office.groupOfStudent("BR-O"), "BR-D1");
  EXPECT_TRUE(office.transferStudent("BR-D1", "BR-D2", "BR-O"));
  EXPECT_FALSE(office.transferStudent("BR-D1", "BR-D2", "BR-O"));
  EXPECT_EQ(office.groupOfStudent("BR-O"), "BR-D2");
  EXPECT_FALSE(office.removeFromGroup("BR-D2", "missing"));
  EXPECT_TRUE(office.removeFromGroup("BR-D2", "BR-O"));
  EXPECT_TRUE(office.expelStudent("BR-O"));
  EXPECT_TRUE(office.removeEmptyGroup("BR-D1"));
  EXPECT_FALSE(office.removeEmptyGroup("BR-D1"));
  EXPECT_FALSE(office.removeEmptyGroup("missing"));
  EXPECT_FALSE(office.groupHasCapacity("missing"));
  EXPECT_TRUE(office.groupHasCapacity("BR-D2"));
  EXPECT_EQ(office.studentsWithAcademicDebts("missing"), 0U);

  Specialty specialty("BR-SP", "Branch specialty", 4);
  office.registerSpecialty(specialty);
  EXPECT_THROW(office.registerSpecialty(specialty), std::invalid_argument);
  EXPECT_NE(office.findSpecialty("BR-SP"), nullptr);
  EXPECT_EQ(office.specialties().size(), 1U);
  EXPECT_FALSE(office.removeSpecialty("missing"));
  EXPECT_TRUE(office.removeSpecialty("BR-SP"));
  EXPECT_EQ(office.specialties().size(), 0U);
}

TEST(BranchLinesFollowup, TeacherSubjectAndCourseNoMatchBranches) {
  Teacher teacher("Teacher", 40, "BR-T", "Math department");
  Subject math = branchSubject();
  EXPECT_FALSE(teacher.teachesSubject("MATH"));
  EXPECT_FALSE(teacher.removeSubject("MATH"));
  teacher.assignSubject(math);
  EXPECT_TRUE(teacher.teachesSubject("MATH"));
  EXPECT_THROW(teacher.assignSubject(math), std::invalid_argument);
  EXPECT_FALSE(teacher.removeSubject("PHYS"));
  EXPECT_TRUE(teacher.removeSubject("MATH"));
  EXPECT_FALSE(teacher.removeCourse("Absent"));
  teacher.assignCourse("Algebra");
  EXPECT_THROW(teacher.assignCourse("Algebra"), std::invalid_argument);
  EXPECT_TRUE(teacher.removeCourse("Algebra"));
  teacher.clearTeachingAssignments();
  EXPECT_TRUE(teacher.courses().empty());
}
