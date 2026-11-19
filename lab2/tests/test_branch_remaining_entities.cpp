#include <gtest/gtest.h>

#include "AcademicOrder.h"
#include "AcademicWork.h"
#include "Assignment.h"
#include "Classroom.h"
#include "Company.h"
#include "Contract.h"
#include "CourseProject.h"
#include "Curriculum.h"
#include "Day.h"
#include "Dean.h"
#include "Exam.h"
#include "ExpulsionOrder.h"
#include "FinancialReport.h"
#include "People.h"
#include "Premy.h"
#include "Scholarship.h"
#include "Secretary.h"
#include "Semester.h"
#include "Specialty.h"
#include "StudentApplication.h"
#include "StudyProgram.h"
#include "Subject.h"
#include "TransferOrder.h"
#include "Week.h"
#include "Worker.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
Subject remainingSubject() { return Subject("REM", "Remaining", 3); }
Classroom remainingRoom() { return Classroom("R-101", 30); }
}

TEST(RemainingBranchCoverage, AcademicOrdersAndDeanApproval) {
  EXPECT_THROW(AcademicOrder(0, "Order"), std::invalid_argument);
  EXPECT_THROW(AcademicOrder(1, ""), std::invalid_argument);
  AcademicOrder order(1, "General order");
  EXPECT_FALSE(order.isApproved());
  Dean dean("Dean", 50, "D-1", "Dean");
  dean.approveOrder(order);
  EXPECT_TRUE(order.isApproved());
  EXPECT_EQ(dean.role(), "Декан");

  EXPECT_THROW(ExpulsionOrder(2, "", "Reason"), std::invalid_argument);
  EXPECT_THROW(ExpulsionOrder(2, "S-1", ""), std::invalid_argument);
  ExpulsionOrder expulsion(2, "S-1", "Violation");
  EXPECT_EQ(expulsion.studentId(), "S-1");
  EXPECT_EQ(expulsion.reason(), "Violation");

  EXPECT_THROW(TransferOrder(3, "", "A", "B"), std::invalid_argument);
  EXPECT_THROW(TransferOrder(3, "S-1", "", "B"), std::invalid_argument);
  EXPECT_THROW(TransferOrder(3, "S-1", "A", "A"), std::invalid_argument);
  TransferOrder transfer(3, "S-1", "A", "B");
  EXPECT_EQ(transfer.fromGroup(), "A");
  EXPECT_EQ(transfer.toGroup(), "B");
}

TEST(RemainingBranchCoverage, AcademicWorkAssignmentAndCourseProject) {
  EXPECT_THROW(AcademicWork("", 10), std::invalid_argument);
  EXPECT_THROW(AcademicWork("Work", 0), std::invalid_argument);
  AcademicWork work("Work", 10);
  EXPECT_EQ(work.type(), "Academic work");

  EXPECT_THROW(Assignment("", "Work"), std::invalid_argument);
  EXPECT_THROW(Assignment("S-1", ""), std::invalid_argument);
  Assignment assignment("S-1", "Work");
  EXPECT_FALSE(assignment.isSubmitted());
  EXPECT_THROW(assignment.submit(""), std::invalid_argument);
  assignment.submit("work.pdf");
  EXPECT_TRUE(assignment.isSubmitted());
  EXPECT_EQ(assignment.fileName(), "work.pdf");

  EXPECT_THROW(CourseProject("Project", 100, ""), std::invalid_argument);
  CourseProject project("Project", 100, "Research");
  EXPECT_EQ(project.type(), "Course project");
  EXPECT_THROW(project.addMilestone(""), std::invalid_argument);
  project.addMilestone("Analysis");
  EXPECT_THROW(project.addMilestone("Analysis"), std::invalid_argument);
  project.addMilestone("Implementation");
  EXPECT_EQ(project.milestones().size(), 2U);
}

TEST(RemainingBranchCoverage, CurriculumAndStudyProgramAlternatives) {
  EXPECT_THROW(Curriculum("", 100), std::invalid_argument);
  EXPECT_THROW(Curriculum("C-1", 0), std::invalid_argument);
  Curriculum curriculum("C-1", 100);
  EXPECT_FALSE(curriculum.containsSubject("Math"));
  EXPECT_EQ(curriculum.hoursFor("Math"), 0);
  EXPECT_THROW(curriculum.addSubject("", 10), std::invalid_argument);
  EXPECT_THROW(curriculum.addSubject("Math", 0), std::invalid_argument);
  curriculum.addSubject("Math", 40);
  EXPECT_TRUE(curriculum.containsSubject("Math"));
  EXPECT_EQ(curriculum.hoursFor("Math"), 40);
  EXPECT_THROW(curriculum.addSubject("Math", 10), std::invalid_argument);
  EXPECT_THROW(curriculum.addSubject("Physics", 70), std::invalid_argument);

  EXPECT_THROW(Specialty("", "Name", 4), std::invalid_argument);
  EXPECT_THROW(Specialty("SP", "", 4), std::invalid_argument);
  EXPECT_THROW(Specialty("SP", "Name", 0), std::invalid_argument);
  Specialty specialty("SP", "Software", 4);
  EXPECT_FALSE(specialty.hasCurriculum());
  specialty.attachCurriculum(curriculum);
  EXPECT_TRUE(specialty.hasCurriculum());

  EXPECT_THROW(StudyProgram("", 2026), std::invalid_argument);
  EXPECT_THROW(StudyProgram("Program", 0), std::invalid_argument);
  StudyProgram program("Program", 2026);
  EXPECT_FALSE(program.offers("SP"));
  program.addSpecialty(specialty);
  EXPECT_TRUE(program.offers("SP"));
  EXPECT_THROW(program.addSpecialty(specialty), std::invalid_argument);
}

TEST(RemainingBranchCoverage, SemesterAndStudentApplication) {
  EXPECT_THROW(Semester(0, 2026), std::invalid_argument);
  EXPECT_THROW(Semester(3, 2026), std::invalid_argument);
  EXPECT_THROW(Semester(1, 0), std::invalid_argument);
  Semester semester(1, 2026);
  Subject subject = remainingSubject();
  EXPECT_FALSE(semester.hasSubject("REM"));
  semester.addSubject(subject);
  EXPECT_TRUE(semester.hasSubject("REM"));
  EXPECT_THROW(semester.addSubject(subject), std::invalid_argument);

  EXPECT_THROW(StudentApplication("", "Topic"), std::invalid_argument);
  EXPECT_THROW(StudentApplication("S-1", ""), std::invalid_argument);
  StudentApplication application("S-1", "Leave");
  EXPECT_FALSE(application.reviewed());
  application.markReviewed();
  EXPECT_TRUE(application.reviewed());
}

TEST(RemainingBranchCoverage, FinancialReportAndAwards) {
  EXPECT_THROW(FinancialReport(""), std::invalid_argument);
  FinancialReport report("2026-Q4");
  EXPECT_DOUBLE_EQ(report.balance(), 0.0);
  EXPECT_THROW(report.addIncome(0), std::invalid_argument);
  EXPECT_THROW(report.addExpense(-1), std::invalid_argument);
  report.addIncome(1000);
  report.addExpense(250);
  EXPECT_DOUBLE_EQ(report.balance(), 750.0);

  EXPECT_THROW(Premy("", 10), std::invalid_argument);
  EXPECT_THROW(Premy("Award", -1), std::invalid_argument);
  Premy award("Award", 10);
  EXPECT_THROW(award.changeAmount(-1), std::invalid_argument);
  award.changeAmount(25);
  EXPECT_DOUBLE_EQ(award.amount(), 25.0);
  Scholarship scholarship("Academic", 100);
  scholarship.changeAmount(200);
  EXPECT_DOUBLE_EQ(scholarship.amount(), 200.0);
}

TEST(RemainingBranchCoverage, PeopleWorkerAndSecretaryValidation) {
  EXPECT_THROW(People("Invalid", -1), std::invalid_argument);
  People person("Person", 20);
  EXPECT_EQ(person.role(), "Человек");
  person.changeName("Renamed");
  EXPECT_EQ(person.fullName(), "Renamed");

  EXPECT_THROW(Worker("Worker", 30, "", "Position"), std::invalid_argument);
  EXPECT_THROW(Worker("Worker", 30, "W-1", ""), std::invalid_argument);
  Worker worker("Worker", 30, "W-1", "Assistant");
  EXPECT_EQ(worker.role(), "Сотрудник");
  EXPECT_THROW(worker.changePosition(""), std::invalid_argument);
  worker.changePosition("Senior assistant");
  EXPECT_EQ(worker.position(), "Senior assistant");

  Secretary secretary("Secretary", 35, "SEC-1");
  EXPECT_THROW(secretary.registerDocument(""), std::invalid_argument);
  EXPECT_NE(secretary.registerDocument("Order 15").empty(), true);
}

TEST(RemainingBranchCoverage, ClassroomAndWeekLimits) {
  EXPECT_THROW(Classroom("", 20), std::invalid_argument);
  EXPECT_THROW(Classroom("R-1", 0), std::invalid_argument);
  Classroom room = remainingRoom();
  EXPECT_TRUE(room.canFit(30));
  EXPECT_FALSE(room.canFit(31));
  EXPECT_THROW(room.canFit(-1), std::invalid_argument);

  EXPECT_THROW(Week(0), std::invalid_argument);
  Week week(1);
  for (int i = 0; i < 7; ++i) {
    week.addDay(Day("Day" + std::to_string(i), "2026-11-" + std::to_string(10 + i)));
  }
  EXPECT_THROW(week.addDay(Day("Extra", "2026-11-20")), std::length_error);
  EXPECT_TRUE(week.hasDay("2026-11-10"));
  EXPECT_FALSE(week.hasDay("2026-11-30"));
  EXPECT_FALSE(week.removeDay("2026-11-30"));
  EXPECT_TRUE(week.removeDay("2026-11-10"));
}

TEST(RemainingBranchCoverage, ContractRequiresTwoDistinctCompanies) {
  Contract contract;
  Company first("Contract", "First");
  Company second("Contract", "Second");
  EXPECT_THROW(contract.makeContract(), std::logic_error);
  contract.addCompany(first);
  EXPECT_THROW(contract.addCompany(first), std::invalid_argument);
  EXPECT_THROW(contract.makeContract(), std::logic_error);
  contract.addCompany(second);
  contract.makeContract();
  EXPECT_TRUE(contract.isMade());
}
