#include <gtest/gtest.h>

#include "AcademicOrder.h"
#include "AcademicWork.h"
#include "AccountingDepartment.h"
#include "Assignment.h"
#include "Benefit.h"
#include "Classroom.h"
#include "Company.h"
#include "Consultation.h"
#include "Contract.h"
#include "CourseProject.h"
#include "Curator.h"
#include "Curriculum.h"
#include "Day.h"
#include "Dean.h"
#include "DeanOffice.h"
#include "Evaluation.h"
#include "Exam.h"
#include "ExpulsionOrder.h"
#include "FinancialReport.h"
#include "Group.h"
#include "LaboratoryWork.h"
#include "Lecture.h"
#include "Lesson.h"
#include "Mark.h"
#include "PaymentRecord.h"
#include "People.h"
#include "Perezdacha.h"
#include "Premy.h"
#include "Schedule.h"
#include "Scholarship.h"
#include "ScholarshipApplication.h"
#include "Secretary.h"
#include "Seminar.h"
#include "Semester.h"
#include "Specialty.h"
#include "Student.h"
#include "StudentApplication.h"
#include "StudyProgram.h"
#include "Subject.h"
#include "Submission.h"
#include "Teacher.h"
#include "TransferOrder.h"
#include "Week.h"
#include "Worker.h"
#include "exceptions.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
Subject sMath() { return Subject("MATH", "Mathematics", 5); }
Classroom sRoom() { return Classroom("R-101", 40); }
std::shared_ptr<Lesson> sLecture(const std::string &teacher = "T-1",
                                 const std::string &day = "Monday",
                                 int start = 9, int duration = 2) {
  return std::make_shared<Lecture>(sMath(), teacher, day, start, duration,
                                   sRoom());
}
}

TEST(AllMissingBranches, SimpleEntitiesAndConstructorAlternatives) {
  EXPECT_THROW(AcademicOrder(0, "Order"), std::invalid_argument);
  EXPECT_THROW(AcademicOrder(1, ""), std::invalid_argument);
  Dean dean("Dean", 50, "D-1", "Dean");
  AcademicOrder order(1, "Order");
  EXPECT_FALSE(order.isApproved());
  dean.approveOrder(order);
  EXPECT_TRUE(order.isApproved());

  EXPECT_THROW(AcademicWork("", 10), std::invalid_argument);
  EXPECT_THROW(AcademicWork("Work", 0), std::invalid_argument);
  EXPECT_THROW(Assignment("", "Work"), std::invalid_argument);
  EXPECT_THROW(Assignment("S", ""), std::invalid_argument);
  Assignment assignment("S", "Work");
  EXPECT_FALSE(assignment.isSubmitted());
  EXPECT_THROW(assignment.submit(""), std::invalid_argument);
  assignment.submit("work.pdf");
  EXPECT_TRUE(assignment.isSubmitted());
  EXPECT_EQ(assignment.fileName(), "work.pdf");

  EXPECT_THROW(Classroom("", 10), std::invalid_argument);
  EXPECT_THROW(Classroom("R", 0), std::invalid_argument);
  EXPECT_THROW(sRoom().canFit(-1), std::invalid_argument);
  EXPECT_TRUE(sRoom().canFit(40));
  EXPECT_FALSE(sRoom().canFit(41));

  EXPECT_THROW(Subject("", "Math", 1), std::invalid_argument);
  EXPECT_THROW(Subject("M", "", 1), std::invalid_argument);
  EXPECT_THROW(Subject("M", "Math", 0), std::invalid_argument);
  EXPECT_THROW(People("P", -1), std::invalid_argument);
  People person("Person", 20);
  person.changeName("Changed");
  EXPECT_EQ(person.fullName(), "Changed");
  EXPECT_EQ(person.role(), "Человек");

  EXPECT_THROW(Worker("W", 20, "", "Position"), std::invalid_argument);
  EXPECT_THROW(Worker("W", 20, "W-1", ""), std::invalid_argument);
  Worker worker("Worker", 30, "W-1", "Assistant");
  EXPECT_THROW(worker.changePosition(""), std::invalid_argument);
  worker.changePosition("Manager");
  EXPECT_EQ(worker.position(), "Manager");
  EXPECT_THROW(Dean("D", 40, "", "Dean"), std::invalid_argument);
  EXPECT_THROW(Secretary("S", 30, ""), std::invalid_argument);
  Secretary secretary("Secretary", 30, "SEC-1");
  EXPECT_THROW(secretary.registerDocument(""), std::invalid_argument);
  EXPECT_FALSE(secretary.registerDocument("Order 5").empty());
}

TEST(AllMissingBranches, BenefitScholarshipAndStudentStateTransitions) {
  EXPECT_THROW(Benefit("Benefit", 10, ""), std::invalid_argument);
  EXPECT_THROW(Benefit("Benefit", 10, "Desc").assignTo(""), std::invalid_argument);
  Benefit benefit("Social", 100, "Support");
  EXPECT_FALSE(benefit.assignedTo("S-1"));
  benefit.assignTo("S-1");
  EXPECT_TRUE(benefit.assignedTo("S-1"));
  EXPECT_THROW(benefit.assignTo("S-1"), std::invalid_argument);
  EXPECT_THROW(Premy("", 1), std::invalid_argument);
  EXPECT_THROW(Premy("Prize", -1), std::invalid_argument);
  Premy prize("Prize", 10);
  EXPECT_THROW(prize.changeAmount(-1), std::invalid_argument);
  prize.changeAmount(0);
  EXPECT_DOUBLE_EQ(prize.amount(), 0);
  Scholarship scholarship("Merit", 500);
  scholarship.changeAmount(750);
  EXPECT_DOUBLE_EQ(scholarship.amount(), 750);

  Student student("Student", 20, "S-1", 1);
  Benefit studentBenefit("Student support", 50, "For student");
  EXPECT_TRUE(student.isEligibleForScholarship());
  student.assignBenefit(studentBenefit);
  EXPECT_THROW(student.assignBenefit(benefit), std::invalid_argument);
  student.removeBenefit();
  EXPECT_THROW(student.removeBenefit(), std::invalid_argument);
  student.awardScholarship();
  student.awardScholarship();
  EXPECT_TRUE(student.isEligibleForScholarship());
  student.removeScholarship();
  EXPECT_TRUE(student.isEligibleForScholarship());
  EXPECT_THROW(student.countMarksBelow(-1), std::invalid_argument);
  EXPECT_THROW(student.countMarksBelow(11), std::invalid_argument);
  EXPECT_THROW(student.recalculateAcademicDebtCount(-1), std::invalid_argument);
  EXPECT_THROW(student.recalculateAcademicDebtCount(11), std::invalid_argument);
  student.setAcademicDebtCount(2);
  EXPECT_FALSE(student.isEligibleForScholarship());
  student.setAcademicDebtCount(-2);
  EXPECT_EQ(student.academicDebtCount(), 2);
  EXPECT_THROW(student.setSubGroup(-1), std::invalid_argument);
  student.setSubGroup(2);
  EXPECT_EQ(student.subGroupNum(), 2);

  Day date("Monday", "2026-11-16");
  Subject math = sMath();
  student.addMark(Mark(2, math, date));
  student.addMark(Mark(8, math, date));
  EXPECT_TRUE(student.hasMarkForSubject("MATH"));
  EXPECT_FALSE(student.hasMarkForSubject("PHYS"));
  EXPECT_DOUBLE_EQ(student.averageForSubject("MATH"), 5.0);
  EXPECT_DOUBLE_EQ(student.averageForSubject("PHYS"), 0.0);
  EXPECT_EQ(student.recalculateAcademicDebtCount(5), 1U);
  EXPECT_EQ(student.removeMarksForSubject("PHYS"), 0U);
  EXPECT_EQ(student.removeMarksForSubject("MATH"), 2U);
  EXPECT_DOUBLE_EQ(student.averageMarkCheck(), 0.0);

  EXPECT_THROW(StudentApplication("", "Topic"), std::invalid_argument);
  EXPECT_THROW(StudentApplication("S-1", ""), std::invalid_argument);
  student.submitApplication(StudentApplication("S-1", "Topic"));
  EXPECT_FALSE(student.reviewApplication("Absent"));
  EXPECT_TRUE(student.reviewApplication("Topic"));
  EXPECT_FALSE(student.reviewApplication("Topic"));

  EXPECT_THROW(ScholarshipApplication("", "Merit"), std::invalid_argument);
  EXPECT_THROW(ScholarshipApplication("S-1", ""), std::invalid_argument);
  ScholarshipApplication rejected("S-1", "Need");
  EXPECT_THROW(rejected.reject(""), std::invalid_argument);
  rejected.reject("Reason");
  EXPECT_THROW(rejected.reject("Again"), std::logic_error);
  EXPECT_THROW(rejected.approve(), std::logic_error);
  ScholarshipApplication approved("S-1", "Merit");
  approved.approve();
  EXPECT_THROW(approved.approve(), std::logic_error);
}

TEST(AllMissingBranches, AcademicWorksOrdersAndSubmissions) {
  EXPECT_THROW(CourseProject("", 10, "Topic"), std::invalid_argument);
  EXPECT_THROW(CourseProject("Project", 10, ""), std::invalid_argument);
  CourseProject project("Project", 100, "Topic");
  EXPECT_THROW(project.addMilestone(""), std::invalid_argument);
  project.addMilestone("Plan");
  EXPECT_THROW(project.addMilestone("Plan"), std::invalid_argument);
  EXPECT_EQ(project.milestones().size(), 1U);

  EXPECT_THROW(Submission("", "Work"), std::invalid_argument);
  EXPECT_THROW(Submission("S-1", ""), std::invalid_argument);
  Submission submission("S-1", "Work");
  EXPECT_FALSE(submission.graded());
  EXPECT_THROW(submission.recordScore(0, 0), std::invalid_argument);
  EXPECT_THROW(submission.recordScore(-1, 10), std::out_of_range);
  EXPECT_THROW(submission.recordScore(11, 10), std::out_of_range);
  submission.recordScore(0, 10);
  EXPECT_TRUE(submission.graded());

  EXPECT_THROW(TransferOrder(0, "S", "A", "B"), std::invalid_argument);
  EXPECT_THROW(TransferOrder(1, "", "A", "B"), std::invalid_argument);
  EXPECT_THROW(TransferOrder(1, "S", "", "B"), std::invalid_argument);
  EXPECT_THROW(TransferOrder(1, "S", "A", "A"), std::invalid_argument);
  TransferOrder transfer(1, "S", "A", "B");
  EXPECT_EQ(transfer.toGroup(), "B");
  EXPECT_THROW(ExpulsionOrder(1, "", "Reason"), std::invalid_argument);
  EXPECT_THROW(ExpulsionOrder(1, "S", ""), std::invalid_argument);
  ExpulsionOrder expulsion(2, "S", "Reason");
  EXPECT_EQ(expulsion.studentId(), "S");

  Evaluation evaluation("T", "Initial");
  EXPECT_THROW(evaluation.reviseComment(""), std::invalid_argument);
  evaluation.reviseComment("Updated");
  EXPECT_EQ(evaluation.comment(), "Updated");
}

TEST(AllMissingBranches, AccountingAllFiltersAndMutations) {
  EXPECT_THROW(AccountingDepartment(""), std::invalid_argument);
  AccountingDepartment acc("Accounting");
  EXPECT_DOUBLE_EQ(acc.totalPaid(), 0.0);
  EXPECT_DOUBLE_EQ(acc.totalUnpaid(), 0.0);
  EXPECT_DOUBLE_EQ(acc.totalPaidForStudent("none"), 0.0);
  EXPECT_DOUBLE_EQ(acc.totalPaidByStudent(Student("Empty", 20, "none", 1)), 0.0);
  EXPECT_TRUE(acc.paymentsForStudent("none").empty());
  EXPECT_FALSE(acc.hasPayment("none", "fee"));
  EXPECT_EQ(acc.markStudentPaymentsPaid("none"), 0U);
  EXPECT_EQ(acc.removeUnpaidPayments("none"), 0U);
  EXPECT_EQ(acc.cancelUnpaidPaymentsForPurpose("none", "fee"), 0U);
  EXPECT_EQ(acc.settleUnpaidPaymentsForPurpose("none", "fee"), 0U);

  acc.recordPayment(PaymentRecord("S1", 10, "Tuition", "d1"));
  acc.recordPayment(PaymentRecord("S1", 20, "Books", "d2"));
  acc.recordPayment(PaymentRecord("S2", 30, "Tuition", "d3"));
  PaymentRecord paid("S1", 40, "Tuition", "d4");
  paid.markPaid();
  acc.recordPayment(paid);
  EXPECT_DOUBLE_EQ(acc.totalUnpaid(), 60.0);
  EXPECT_DOUBLE_EQ(acc.totalPaid(), 40.0);
  EXPECT_DOUBLE_EQ(acc.totalPaidForStudent("S1"), 40.0);
  EXPECT_DOUBLE_EQ(acc.totalForPurpose("Tuition", false), 80.0);
  EXPECT_DOUBLE_EQ(acc.totalForPurpose("Tuition", true), 40.0);
  EXPECT_EQ(acc.markStudentPaymentsPaid("S1"), 2U);
  EXPECT_EQ(acc.markStudentPaymentsPaid("S1"), 0U);
  EXPECT_EQ(acc.settleUnpaidPaymentsForPurpose("S2", "Tuition"), 1U);
  EXPECT_EQ(acc.settleUnpaidPaymentsForPurpose("S2", "Other"), 0U);
  EXPECT_EQ(acc.paymentsForStudent("S1").size(), 3U);
  Student s("Student", 19, "S1", 1);
  EXPECT_EQ(acc.paymentCountForStudent(s), 3U);
  EXPECT_DOUBLE_EQ(acc.totalPaidByStudent(s), 70.0);
  EXPECT_EQ(acc.makeReport("November").period(), "November");
  EXPECT_EQ(acc.removeUnpaidPayments("S1"), 0U);
  acc.recordPayment(PaymentRecord("S1", 5, "Dorm", "d5"));
  acc.recordPayment(PaymentRecord("S1", 6, "Dorm", "d6"));
  EXPECT_EQ(acc.cancelUnpaidPaymentsForPurpose("S1", "Dorm"), 2U);
  EXPECT_EQ(acc.paymentCount(), 4U);
  EXPECT_EQ(acc.name(), "Accounting");
}

TEST(AllMissingBranches, CompanyContractAndCurriculumAlternatives) {
  EXPECT_THROW(Company("", "A"), std::invalid_argument);
  EXPECT_THROW(Company("Type", ""), std::invalid_argument);
  Company a("Service", "A"), b("Service", "B");
  EXPECT_THROW(a.addPartner(nullptr), std::invalid_argument);
  auto bp = std::make_shared<Company>(b);
  a.addPartner(bp);
  EXPECT_THROW(a.addPartner(bp), std::invalid_argument);
  EXPECT_TRUE(a.hasPartner("B"));
  EXPECT_FALSE(a.hasPartner("C"));
  EXPECT_FALSE(a.removePartner("C"));
  EXPECT_TRUE(a.removePartner("B"));
  EXPECT_FALSE(a.removePartner("B"));

  Contract contract;
  EXPECT_THROW(contract.makeContract(), std::logic_error);
  contract.addCompany(a);
  EXPECT_THROW(contract.addCompany(a), std::invalid_argument);
  contract.addCompany(b);
  contract.makeContract();
  EXPECT_TRUE(contract.isMade());

  EXPECT_THROW(Curriculum("", 10), std::invalid_argument);
  EXPECT_THROW(Curriculum("C", 0), std::invalid_argument);
  Curriculum curriculum("C-1", 100);
  EXPECT_THROW(curriculum.addSubject("", 10), std::invalid_argument);
  EXPECT_THROW(curriculum.addSubject("Math", 0), std::invalid_argument);
  curriculum.addSubject("Math", 40);
  EXPECT_THROW(curriculum.addSubject("Math", 10), std::invalid_argument);
  EXPECT_TRUE(curriculum.containsSubject("Math"));
  EXPECT_FALSE(curriculum.containsSubject("Physics"));
  EXPECT_EQ(curriculum.hoursFor("Physics"), 0);
  EXPECT_EQ(curriculum.hoursFor("Math"), 40);

  EXPECT_THROW(Specialty("", "Name", 4), std::invalid_argument);
  EXPECT_THROW(Specialty("S", "", 4), std::invalid_argument);
  EXPECT_THROW(Specialty("S", "Name", 0), std::invalid_argument);
  Specialty specialty("CS", "Computer Science", 4);
  EXPECT_FALSE(specialty.hasCurriculum());
  specialty.attachCurriculum(curriculum);
  EXPECT_TRUE(specialty.hasCurriculum());
  EXPECT_THROW(StudyProgram("", 2026), std::invalid_argument);
  EXPECT_THROW(StudyProgram("Program", 0), std::invalid_argument);
  StudyProgram program("Program", 2026);
  program.addSpecialty(specialty);
  EXPECT_TRUE(program.offers("CS"));
  EXPECT_FALSE(program.offers("MATH"));
  EXPECT_THROW(program.addSpecialty(specialty), std::invalid_argument);
}

TEST(AllMissingBranches, GroupInsertionRemovalAndOrdering) {
  EXPECT_THROW(Group("", "CS", 2), std::invalid_argument);
  EXPECT_THROW(Group("G", "", 2), std::invalid_argument);
  EXPECT_THROW(Group("G", "CS", 0), std::invalid_argument);
  Group group("G", "CS", 4);
  EXPECT_TRUE(group.hasCapacity());
  EXPECT_EQ(group.availableSeats(), 4U);
  EXPECT_FALSE(group.containsStudent("S"));
  EXPECT_EQ(group.findStudent("S"), nullptr);
  EXPECT_FALSE(group["S"]);
  EXPECT_FALSE(group.changeStudentSubGroup("S", 1));
  EXPECT_FALSE(group.changeStudentSubGroup("S", 0));
  EXPECT_THROW(group.assignSubGroup(0), std::invalid_argument);
  EXPECT_TRUE(group.studentsInSubGroup(0).empty());
  EXPECT_THROW(group.studentsInSubGroup(-1), std::invalid_argument);
  Student b("Beta", 20, "B", 1), a("Alpha", 20, "A", 1);
  group += b;
  group += a;
  EXPECT_TRUE(group.containsStudent("A"));
  EXPECT_EQ(group.findStudent("A")->studentId(), "A");
  EXPECT_EQ(group.unassignedStudentCount(), 2U);
  EXPECT_EQ(group.assignSubGroup(1), 2U);
  EXPECT_EQ(group.studentsInSubGroup(1).size(), 2U);
  EXPECT_TRUE(group.changeStudentSubGroup("A", 2));
  EXPECT_EQ(group.studentsInSubGroup(2).size(), 1U);
  group.sortByName();
  EXPECT_EQ(group.students().front()->studentId(), "A");
  group.sortByAverageMark();
  EXPECT_FALSE(group.changeStudentSubGroup("missing", 2));
  EXPECT_FALSE(group -= "missing");
  EXPECT_TRUE(group -= "A");
  EXPECT_EQ(group.size(), 1U);
  EXPECT_EQ(group.availableSeats(), 3U);
  EXPECT_THROW(group += std::shared_ptr<Student>{}, std::invalid_argument);
}

TEST(AllMissingBranches, DayWeekScheduleAndLessonDerivedTypes) {
  EXPECT_THROW(Day("", "date"), std::invalid_argument);
  EXPECT_THROW(Day("Monday", ""), std::invalid_argument);
  Day day("Monday", "2026-11-16");
  EXPECT_THROW(day.addLesson(nullptr), std::invalid_argument);
  auto lecture = sLecture();
  auto seminar = std::make_shared<Seminar>(sMath(), "T-2", "Monday", 12, 2,
                                           sRoom());
  auto exam = std::make_shared<Exam>(sMath(), "T-3", "Monday", 15, 2,
                                     sRoom(), "Written");
  EXPECT_THROW(Exam(sMath(), "T", "Monday", 9, 1, sRoom(), ""), std::invalid_argument);
  day.addLesson(lecture);
  day.addLesson(seminar);
  day.addLesson(exam);
  EXPECT_TRUE(day.hasTeacher("T-1"));
  EXPECT_FALSE(day.hasTeacher("absent"));
  EXPECT_TRUE(day.hasSubject("MATH"));
  EXPECT_FALSE(day.hasSubject("PHYS"));
  EXPECT_TRUE(day.hasTimeConflict(10, 1));
  EXPECT_FALSE(day.hasTimeConflict(11, 1));
  EXPECT_THROW(day.hasTimeConflict(-1, 1), std::invalid_argument);
  EXPECT_THROW(day.hasTimeConflict(23, 2), std::invalid_argument);
  EXPECT_EQ(day.lessonsByTeacher("T-1").size(), 1U);
  EXPECT_TRUE(day.removeLesson(0));
  EXPECT_FALSE(day.removeLesson(100));
  EXPECT_EQ(day.lessonCount(), 2U);

  EXPECT_THROW(Week(0), std::invalid_argument);
  Week week(1);
  EXPECT_FALSE(week.hasDay("none"));
  EXPECT_FALSE(week.removeDay("none"));
  week.addDay(day);
  EXPECT_THROW(week.addDay(day), std::invalid_argument);
  EXPECT_TRUE(week.hasDay("2026-11-16"));
  EXPECT_TRUE(week.hasTeacher("T-2"));
  EXPECT_FALSE(week.hasTeacher("absent"));
  EXPECT_TRUE(week.hasTimeConflict("Monday", 12, 1));
  EXPECT_FALSE(week.hasTimeConflict("Tuesday", 12, 1));
  EXPECT_TRUE(week.removeDay("2026-11-16"));
  EXPECT_FALSE(week.removeDay("2026-11-16"));

  EXPECT_THROW(Schedule(""), std::invalid_argument);
  Schedule schedule("G-1");
  EXPECT_FALSE(schedule.hasWeek(1));
  EXPECT_FALSE(schedule.removeWeek(1));
  schedule.addWeek(week);
  schedule.addWeek(Week(2));
  EXPECT_TRUE(schedule.hasWeek(1));
  EXPECT_EQ(schedule.removeWeeksWithoutLessons(), 2U);
  EXPECT_EQ(schedule.weekCount(), 0U);
  EXPECT_FALSE(schedule.hasTimeConflict(99, "Monday", 9, 1));
  EXPECT_EQ(schedule.groupCode(), "G-1");

  std::string newType = "Oral";
  exam->examTypeSet(newType);
  EXPECT_EQ(exam->examType(), "Oral");
  std::string emptyType;
  EXPECT_THROW(exam->examTypeSet(emptyType), std::invalid_argument);
  Lecture lectureInfo(sMath(), "T", "Monday", 9, 1, sRoom());
  Seminar seminarInfo(sMath(), "T", "Monday", 10, 1, sRoom());
  lectureInfo.info();
  seminarInfo.info();
  exam->info();
  Consultation consultation(sMath(), "T", "Monday", 11, 1, sRoom(), *exam);
  EXPECT_EQ(consultation.consExam().examType(), "Oral");
  consultation.info();
}

TEST(AllMissingBranches, TeacherCuratorAndDeanOfficeDecisionBranches) {
  Subject math = sMath();
  Teacher teacher("Teacher", 40, "T-1", "Math");
  EXPECT_THROW(Teacher("T", 30, "T-2", ""), std::invalid_argument);
  EXPECT_THROW(teacher.assignCourse(""), std::invalid_argument);
  teacher.assignCourse("Algebra");
  EXPECT_THROW(teacher.assignCourse("Algebra"), std::invalid_argument);
  EXPECT_FALSE(teacher.removeCourse("Absent"));
  EXPECT_FALSE(teacher.replaceCourse("Absent", "Geometry"));
  EXPECT_FALSE(teacher.replaceCourse("Algebra", ""));
  EXPECT_FALSE(teacher.replaceCourse("Algebra", "Algebra"));
  EXPECT_TRUE(teacher.replaceCourse("Algebra", "Geometry"));
  teacher.assignSubject(math);
  EXPECT_THROW(teacher.assignSubject(math), std::invalid_argument);
  EXPECT_FALSE(teacher.removeSubject("PHYS"));
  EXPECT_TRUE(teacher.teachesSubject("MATH"));
  EXPECT_FALSE(teacher.teachesSubject("PHYS"));
  EXPECT_TRUE(teacher.removeSubject("MATH"));
  teacher.clearTeachingAssignments();

  Group initial("G1", "CS", 5);
  Curator curator("Curator", 35, "C-1", initial);
  EXPECT_DOUBLE_EQ(curator.groupAverageMark(), 0.0);
  EXPECT_FALSE(curator.supervises("none"));
  EXPECT_EQ(curator.countSubGroup(1), 0U);
  EXPECT_EQ(curator.studentsWithDebts(), 0U);
  EXPECT_FALSE(curator.removeStudent("none"));
  EXPECT_FALSE(curator.transferToSubGroup("none", 1));
  EXPECT_THROW(curator.assignUnassignedStudentsToSubGroup(0), std::invalid_argument);
  EXPECT_THROW(curator.reassignStudentsWithDebts(0), std::invalid_argument);
  Student one("One", 20, "S1", 1), two("Two", 20, "S2", 1);
  one.setAcademicDebtCount(1);
  one.setSubGroup(1);
  initial += one;
  initial += two;
  curator.assignGroup(initial);
  EXPECT_EQ(curator.assignUnassignedStudentsToSubGroup(2), 1U);
  EXPECT_EQ(curator.reassignStudentsWithDebts(3), 1U);
  EXPECT_EQ(curator.studentsWithDebts(), 1U);
  EXPECT_EQ(curator.countSubGroup(3), 1U);

  DeanOffice office("Dean office");
  EXPECT_THROW(DeanOffice(""), std::invalid_argument);
  EXPECT_THROW(office.addWorker(nullptr), std::invalid_argument);
  auto w = std::make_shared<Worker>("Worker", 30, "W-1", "Staff");
  office.addWorker(w);
  EXPECT_THROW(office.addWorker(w), std::invalid_argument);
  EXPECT_FALSE(office.removeWorker("absent"));
  EXPECT_EQ(office.findWorker("absent"), nullptr);
  EXPECT_EQ(office.totalWorkers(), 1U);
  EXPECT_FALSE(office.assignCuratorToGroup("W-1", "missing"));
  EXPECT_FALSE(office.assignCuratorToGroup("missing", "G1"));
  office.addGroup(initial);
  office.addGroup(initial);
  EXPECT_EQ(office.groupCount(), 1U);
  EXPECT_FALSE(office.groupHasCapacity("missing"));
  EXPECT_TRUE(office.groupHasCapacity("G1"));
  EXPECT_FALSE(office.enrollStudent("missing", Student("X", 20, "X", 1)));
  EXPECT_TRUE(office.enrollStudent("G1", Student("X", 20, "X", 1)));
  EXPECT_FALSE(office.enrollStudent("G1", Student("X", 20, "X", 1)));
  EXPECT_FALSE(office.removeFromGroup("G1", "absent"));
  EXPECT_FALSE(office.transferStudent("G1", "G1", "X"));
  EXPECT_FALSE(office.transferStudent("missing", "G1", "X"));
  EXPECT_FALSE(office.transferStudent("G1", "missing", "X"));
  EXPECT_FALSE(office.transferStudent("G1", "G1", "absent"));
  EXPECT_EQ(office.groupOfStudent("X"), "G1");
  EXPECT_EQ(office.studentsWithAcademicDebts("missing"), 0U);
  EXPECT_EQ(office.totalStudents(), 3U);
  EXPECT_FALSE(office.expelStudent("absent"));
  EXPECT_TRUE(office.removeFromGroup("G1", "X"));
  EXPECT_FALSE(office.removeEmptyGroup("G1"));
  EXPECT_THROW(office.giveScholarship("absent"), std::invalid_argument);
  EXPECT_THROW(office.promoteEligibleStudents("G1", -1), std::invalid_argument);
  EXPECT_THROW(office.promoteEligibleStudents("G1", 11), std::invalid_argument);
  EXPECT_EQ(office.promoteEligibleStudents("absent", 5), 0U);
  EXPECT_FALSE(office.removeSpecialty("absent"));
  EXPECT_EQ(office.findSpecialty("absent"), nullptr);
  Specialty specialty("CS", "Computer Science", 4);
  office.registerSpecialty(specialty);
  EXPECT_THROW(office.registerSpecialty(specialty), std::invalid_argument);
  EXPECT_EQ(office.specialtyCount(), 1U);
  EXPECT_TRUE(office.removeSpecialty("CS"));
  EXPECT_FALSE(office.removeSpecialty("CS"));
  EXPECT_TRUE(office.removeWorker("W-1"));
}
