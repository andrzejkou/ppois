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
#include "CourseProjCheck.h"
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
#include "Scholarship.h"
#include "Schedule.h"
#include "Secretary.h"
#include "Semester.h"
#include "Seminar.h"
#include "Specialty.h"
#include "Student.h"
#include "ScientificWork.h"
#include "StudentApplication.h"
#include "StudyProgram.h"
#include "Subject.h"
#include "Submission.h"
#include "Teacher.h"
#include "TransferOrder.h"
#include "Week.h"
#include "Worker.h"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
Subject mathSubject() { return Subject("MATH", "Mathematics", 5); }
Classroom room() { return Classroom("101", 30); }

std::shared_ptr<Lesson> makeLecture(const std::string &teacher = "T1",
                                    const std::string &day = "Monday",
                                    int start = 9, int duration = 2) {
  return std::make_shared<Lecture>(mathSubject(), teacher, day, start,
                                   duration, room());
}
} // namespace

TEST(AdditionalCoverage, AcademicOrdersAndWorkTypes) {
  Dean dean("Dean Name", 50, "D1", "Dean");
  AcademicOrder order(1, "General order");
  EXPECT_FALSE(order.isApproved());
  dean.approveOrder(order);
  EXPECT_TRUE(order.isApproved());
  EXPECT_EQ(order.number(), 1);
  EXPECT_EQ(order.title(), "General order");

  TransferOrder transfer(2, "S1", "G1", "G2");
  EXPECT_EQ(transfer.studentId(), "S1");
  EXPECT_EQ(transfer.fromGroup(), "G1");
  EXPECT_EQ(transfer.toGroup(), "G2");
  ExpulsionOrder expulsion(3, "S2", "Violation");
  EXPECT_EQ(expulsion.studentId(), "S2");
  EXPECT_EQ(expulsion.reason(), "Violation");
  EXPECT_THROW(TransferOrder(4, "S1", "G1", "G1"), std::invalid_argument);
  EXPECT_THROW(ExpulsionOrder(5, "", "Reason"), std::invalid_argument);

  AcademicWork work("Essay", 10);
  EXPECT_EQ(work.type(), "Academic work");
  EXPECT_EQ(work.title(), "Essay");
  EXPECT_EQ(work.maxScore(), 10);
  CourseProject project("Project", 100, "Dean office system");
  EXPECT_EQ(project.type(), "Course project");
  EXPECT_EQ(project.topic(), "Dean office system");
  project.addMilestone("Analysis");
  EXPECT_EQ(project.milestones().size(), 1U);
  EXPECT_THROW(project.addMilestone("Analysis"), std::invalid_argument);
  EXPECT_THROW(project.addMilestone(""), std::invalid_argument);
  LaboratoryWork laboratory("Lab", 10, 4);
  EXPECT_EQ(laboratory.type(), "Laboratory work");
  EXPECT_EQ(laboratory.number(), 4);
  ScientificWork scientific("Research", 20);
  EXPECT_EQ(scientific.type(), "Academic work");
  EXPECT_THROW(AcademicWork("", 10), std::invalid_argument);
  EXPECT_THROW(AcademicWork("Bad", 0), std::invalid_argument);
}

TEST(AdditionalCoverage, AssignmentsEvaluationsAndApplications) {
  Assignment assignment("S1", "Lab 1");
  EXPECT_FALSE(assignment.isSubmitted());
  EXPECT_THROW(assignment.submit(""), std::invalid_argument);
  assignment.submit("lab1.pdf");
  EXPECT_TRUE(assignment.isSubmitted());
  EXPECT_EQ(assignment.fileName(), "lab1.pdf");

  Evaluation evaluation("T1", "Good work");
  EXPECT_EQ(evaluation.evaluatorId(), "T1");
  EXPECT_EQ(evaluation.comment(), "Good work");
  evaluation.reviseComment("Needs corrections");
  EXPECT_EQ(evaluation.comment(), "Needs corrections");
  EXPECT_THROW(evaluation.reviseComment(""), std::invalid_argument);

  StudentApplication application("S1", "Academic leave");
  EXPECT_FALSE(application.reviewed());
  application.markReviewed();
  EXPECT_TRUE(application.reviewed());
  EXPECT_EQ(application.studentId(), "S1");
  EXPECT_EQ(application.topic(), "Academic leave");

  Submission submission("S1", "Lab 1");
  EXPECT_FALSE(submission.graded());
  EXPECT_THROW(submission.recordScore(11, 10), std::out_of_range);
  EXPECT_THROW(submission.recordScore(5, 0), std::invalid_argument);
  submission.recordScore(8, 10);
  EXPECT_TRUE(submission.graded());
  EXPECT_EQ(submission.score(), 8);
  EXPECT_EQ(submission.studentId(), "S1");
  EXPECT_EQ(submission.workTitle(), "Lab 1");
}

TEST(AdditionalCoverage, CurriculumSpecialtyAndStudyProgram) {
  Curriculum curriculum("CS-2026", 120);
  EXPECT_EQ(curriculum.code(), "CS-2026");
  EXPECT_FALSE(curriculum.containsSubject("Math"));
  EXPECT_EQ(curriculum.hoursFor("Math"), 0);
  curriculum.addSubject("Math", 40);
  curriculum.addSubject("Physics", 30);
  EXPECT_TRUE(curriculum.containsSubject("Math"));
  EXPECT_EQ(curriculum.hoursFor("Physics"), 30);
  EXPECT_EQ(curriculum.totalHours(), 70);
  EXPECT_THROW(curriculum.addSubject("Math", 10), std::invalid_argument);
  EXPECT_THROW(curriculum.addSubject("Chemistry", 0), std::invalid_argument);
  EXPECT_THROW(curriculum.addSubject("History", 100), std::invalid_argument);

  Specialty specialty("CS", "Computer Science", 4);
  EXPECT_FALSE(specialty.hasCurriculum());
  specialty.attachCurriculum(curriculum);
  EXPECT_TRUE(specialty.hasCurriculum());
  EXPECT_EQ(specialty.code(), "CS");
  EXPECT_EQ(specialty.name(), "Computer Science");
  EXPECT_EQ(specialty.durationYears(), 4);

  StudyProgram program("IT", 2026);
  EXPECT_EQ(program.name(), "IT");
  EXPECT_EQ(program.admissionYear(), 2026);
  EXPECT_FALSE(program.offers("CS"));
  program.addSpecialty(specialty);
  EXPECT_TRUE(program.offers("CS"));
  EXPECT_THROW(program.addSpecialty(specialty), std::invalid_argument);
  EXPECT_THROW(Curriculum("", 10), std::invalid_argument);
  EXPECT_THROW(Specialty("CS", "", 4), std::invalid_argument);
  EXPECT_THROW(StudyProgram("IT", 0), std::invalid_argument);
}

TEST(AdditionalCoverage, CompanyAndContractLifecycle) {
  Company university("Education", "University");
  auto partner = std::make_shared<Company>("Technology", "Partner");
  EXPECT_EQ(university.name(), "University");
  EXPECT_EQ(university.contractType(), "Education");
  EXPECT_FALSE(university.hasPartner("Partner"));
  EXPECT_THROW(university.addPartner(nullptr), std::invalid_argument);
  EXPECT_THROW(university.addPartner(std::shared_ptr<Company>(&university,
      [](Company *) {})), std::invalid_argument);
  university.addPartner(partner);
  EXPECT_TRUE(university.hasPartner("Partner"));
  EXPECT_THROW(university.addPartner(partner), std::invalid_argument);
  EXPECT_FALSE(university.removePartner("Unknown"));
  EXPECT_TRUE(university.removePartner("Partner"));
  EXPECT_FALSE(university.hasPartner("Partner"));
  university.printInfo();

  Contract contract;
  EXPECT_FALSE(contract.isMade());
  EXPECT_THROW(contract.makeContract(), std::logic_error);
  contract.addCompany(university);
  EXPECT_THROW(contract.addCompany(university), std::invalid_argument);
  contract.addCompany(*partner);
  contract.makeContract();
  EXPECT_TRUE(contract.isMade());
}

TEST(AdditionalCoverage, FinancialReportsAndAwards) {
  FinancialReport report("2026-Q4");
  EXPECT_EQ(report.period(), "2026-Q4");
  EXPECT_DOUBLE_EQ(report.balance(), 0.0);
  EXPECT_THROW(report.addIncome(0), std::invalid_argument);
  EXPECT_THROW(report.addExpense(-1), std::invalid_argument);
  report.addIncome(1000);
  report.addExpense(250);
  EXPECT_DOUBLE_EQ(report.balance(), 750);

  Premy award("Award", 500);
  EXPECT_EQ(award.name(), "Award");
  EXPECT_DOUBLE_EQ(award.amount(), 500);
  award.changeAmount(700);
  EXPECT_DOUBLE_EQ(award.amount(), 700);
  EXPECT_THROW(award.changeAmount(-1), std::invalid_argument);
  Scholarship scholarship("Academic", 1000);
  scholarship.changeAmount(1200);
  EXPECT_DOUBLE_EQ(scholarship.amount(), 1200);

  Benefit benefit("Social", 300, "Social support");
  EXPECT_FALSE(benefit.assignedTo("S1"));
  benefit.assignTo("S1");
  EXPECT_TRUE(benefit.assignedTo("S1"));
  EXPECT_THROW(benefit.assignTo("S1"), std::invalid_argument);
  EXPECT_THROW(benefit.assignTo(""), std::invalid_argument);
}

TEST(AdditionalCoverage, SemesterAndScheduleEntities) {
  Semester semester(1, 2026);
  EXPECT_EQ(semester.number(), 1);
  EXPECT_EQ(semester.year(), 2026);
  EXPECT_FALSE(semester.hasSubject("MATH"));
  semester.addSubject(mathSubject());
  EXPECT_TRUE(semester.hasSubject("MATH"));
  EXPECT_THROW(semester.addSubject(mathSubject()), std::invalid_argument);
  EXPECT_THROW(Semester(3, 2026), std::invalid_argument);

  Exam exam(mathSubject(), "T1", "Monday", 9, 2, room(), "Final");
  EXPECT_EQ(exam.examType(), "Final");
  std::string retakeType = "Retake";
  exam.examTypeSet(retakeType);
  EXPECT_EQ(exam.examType(), "Retake");
  std::string emptyType;
  EXPECT_THROW(exam.examTypeSet(emptyType), std::invalid_argument);
  exam.info();

  Lecture lecture(mathSubject(), "T1", "Monday", 9, 2, room());
  Seminar seminar(mathSubject(), "T1", "Monday", 11, 1, room());
  CourseProjCheck projectCheck(mathSubject(), "T2", "Tuesday", 12, 1, room());
  lecture.info();
  seminar.info();
  projectCheck.info();

  Perezdacha retake(mathSubject(), "T1", "Friday", 10, 2, room(), "Retake", 2);
  EXPECT_EQ(retake.number(), 2);
  retake.info();

  Consultation consultation(mathSubject(), "T1", "Wednesday", 14, 1,
                            room(), exam);
  EXPECT_EQ(consultation.consExam().examType(), "Retake");
  consultation.info();
}

TEST(AdditionalCoverage, PeopleAndWorkerHierarchy) {
  People person("Person Name", 30);
  EXPECT_EQ(person.fullName(), "Person Name");
  EXPECT_EQ(person.age(), 30);
  EXPECT_EQ(person.role(), "Человек");
  person.changeName("New Name");
  EXPECT_EQ(person.fullName(), "New Name");
  person.printInfo();

  Worker worker("Worker Name", 35, "W1", "Assistant");
  EXPECT_EQ(worker.employeeId(), "W1");
  EXPECT_EQ(worker.position(), "Assistant");
  EXPECT_EQ(worker.role(), "Сотрудник");
  worker.changePosition("Senior assistant");
  EXPECT_EQ(worker.position(), "Senior assistant");
  EXPECT_THROW(worker.changePosition(""), std::invalid_argument);
  worker.printInfo();

  Dean dean("Dean Name", 55, "D1", "Dean");
  EXPECT_EQ(dean.role(), "Декан");
  dean.printInfo();
  Secretary secretary("Secretary Name", 40, "SEC1");
  EXPECT_NE(secretary.registerDocument("Order"), "");
  EXPECT_THROW(secretary.registerDocument(""), std::invalid_argument);
  secretary.printInfo();

  Teacher teacher("Teacher Name", 42, "T1", "Math department");
  EXPECT_EQ(teacher.department(), "Math department");
  EXPECT_EQ(teacher.role(), "Преподаватель");
  teacher.assignCourse("Algebra");
  EXPECT_THROW(teacher.assignCourse("Algebra"), std::invalid_argument);
  EXPECT_THROW(teacher.assignCourse(""), std::invalid_argument);
  EXPECT_TRUE(teacher.replaceCourse("Algebra", "Geometry"));
  EXPECT_FALSE(teacher.replaceCourse("Missing", "Other"));
  EXPECT_FALSE(teacher.replaceCourse("Geometry", "Geometry"));
  EXPECT_FALSE(teacher.removeCourse("Missing"));
  EXPECT_TRUE(teacher.removeCourse("Geometry"));
  teacher.assignSubject(mathSubject());
  EXPECT_TRUE(teacher.teachesSubject("MATH"));
  EXPECT_EQ(teacher.subjectCount(), 1U);
  EXPECT_THROW(teacher.assignSubject(mathSubject()), std::invalid_argument);
  EXPECT_FALSE(teacher.removeSubject("PHYS"));
  EXPECT_TRUE(teacher.removeSubject("MATH"));
  teacher.assignCourse("Physics");
  teacher.clearTeachingAssignments();
  EXPECT_TRUE(teacher.courses().empty());
  teacher.printInfo();
}

TEST(AdditionalCoverage, DayWeekAndScheduleRemainingBranches) {
  Day day("Monday", "2026-11-02");
  EXPECT_EQ(day.name(), "Monday");
  EXPECT_EQ(day.getDate(), "2026-11-02");
  EXPECT_FALSE(day.removeLesson(0));
  day.addLesson(makeLecture("T1", "Monday", 9, 2));
  day.addLesson(std::make_shared<Seminar>(mathSubject(), "T2", "Monday", 12,
                                          1, room()));
  EXPECT_TRUE(day.hasTeacher("T1"));
  EXPECT_FALSE(day.hasTeacher("T9"));
  EXPECT_TRUE(day.hasSubject("MATH"));
  EXPECT_FALSE(day.hasSubject("PHYS"));
  EXPECT_TRUE(day.hasTimeConflict(10, 1));
  EXPECT_FALSE(day.hasTimeConflict(14, 1));
  EXPECT_EQ(day.lessonsByTeacher("T1").size(), 1U);
  EXPECT_TRUE(day.removeLesson(1));
  EXPECT_FALSE(day.removeLesson(1));

  Week week(1);
  EXPECT_EQ(week.number(), 1);
  EXPECT_FALSE(week.hasDay("2026-11-02"));
  week.addDay(day);
  EXPECT_TRUE(week.hasDay("2026-11-02"));
  EXPECT_EQ(week.dayCount(), 1U);
  EXPECT_EQ(week.lessonCount(), 1U);
  EXPECT_TRUE(week.hasTeacher("T1"));
  EXPECT_FALSE(week.hasTeacher("T9"));
  EXPECT_TRUE(week.hasTimeConflict("Monday", 9, 1));
  EXPECT_FALSE(week.hasTimeConflict("Tuesday", 9, 1));
  EXPECT_THROW(week.addDay(day), std::invalid_argument);
  EXPECT_FALSE(week.removeDay("missing"));
  EXPECT_TRUE(week.removeDay("2026-11-02"));

  Schedule schedule("G1");
  EXPECT_EQ(schedule.groupCode(), "G1");
  schedule.addWeek(week);
  Week week2(2);
  Day day2("Tuesday", "2026-11-03");
  day2.addLesson(makeLecture("T2", "Tuesday", 10, 2));
  week2.addDay(day2);
  schedule.addWeek(week2);
  EXPECT_EQ(schedule.weekCount(), 2U);
  schedule.changeSchedule();
  const auto orderedWeeks = schedule.weekNumbers();
  EXPECT_EQ(orderedWeeks.front(), 1);
  EXPECT_EQ(schedule.lessonCount(), 1U);
  EXPECT_TRUE(schedule.hasWeek(2));
  EXPECT_FALSE(schedule.hasWeek(9));
  EXPECT_TRUE(schedule.hasTeacher("T2"));
  EXPECT_FALSE(schedule.hasTeacher("T9"));
  EXPECT_TRUE(schedule.hasTimeConflict(2, "Tuesday", 11, 1));
  EXPECT_FALSE(schedule.hasTimeConflict(2, "Monday", 11, 1));
  EXPECT_FALSE(schedule.removeWeek(9));
  EXPECT_TRUE(schedule.removeWeek(2));
  EXPECT_EQ(schedule.removeWeeksWithoutLessons(), 1U);
  schedule.changeSchedule();
  EXPECT_EQ(schedule.weekNumbers().size(), 0U);
}

TEST(AdditionalCoverage, GroupAndCuratorRemainingBranches) {
  Group group("G1", "CS", 5);
  Student alice("Alice", 20, "S1", 2);
  Student bob("Bob", 21, "S2", 2);
  alice.addMark(Mark(9, mathSubject(), Day("Mon", "2026-11-02")));
  bob.addMark(Mark(4, mathSubject(), Day("Mon", "2026-11-02")));
  bob.setAcademicDebtCount(1);
  group += alice;
  group += std::make_shared<Student>(bob);
  EXPECT_EQ(group.size(), 2U);
  EXPECT_EQ(group.availableSeats(), 3U);
  EXPECT_TRUE(group.hasCapacity());
  EXPECT_TRUE(group.containsStudent("S1"));
  EXPECT_FALSE(group.containsStudent("S9"));
  EXPECT_NE(group.findStudent("S1"), nullptr);
  EXPECT_EQ(group.findStudent("S9"), nullptr);
  EXPECT_EQ(group.unassignedStudentCount(), 2U);
  EXPECT_EQ(group.studentsInSubGroup(0).size(), 2U);
  EXPECT_THROW(group.studentsInSubGroup(-1), std::invalid_argument);
  EXPECT_EQ(group.assignSubGroup(1), 2U);
  EXPECT_EQ(group.studentsInSubGroup(1).size(), 2U);
  EXPECT_TRUE(group.changeStudentSubGroup("S1", 2));
  EXPECT_FALSE(group.changeStudentSubGroup("S9", 2));
  group.sortByName();
  group.sortByAverageMark();
  EXPECT_TRUE(group["S1"]);
  EXPECT_FALSE(group["S9"]);
  EXPECT_TRUE(group -= "S2");
  EXPECT_FALSE(group -= "S9");
  EXPECT_THROW(group += std::shared_ptr<Student>(), std::invalid_argument);

  Group curatorGroup("CG", "CS", 5);
  Student unassigned("Unassigned", 20, "C1", 2);
  Student indebted("Indebted", 20, "C2", 2);
  indebted.setAcademicDebtCount(2);
  indebted.addMark(Mark(5, mathSubject(), Day("Mon", "2026-11-02")));
  curatorGroup += unassigned;
  curatorGroup += indebted;
  Curator curator("Curator Name", 45, "CUR1", curatorGroup);
  EXPECT_TRUE(curator.supervises("C1"));
  EXPECT_FALSE(curator.supervises("C9"));
  EXPECT_EQ(curator.studentCount(), 2U);
  EXPECT_EQ(curator.studentsWithDebts(), 1U);
  EXPECT_EQ(curator.groupAverageMark(), 2.5);
  EXPECT_EQ(curator.assignUnassignedStudentsToSubGroup(1), 2U);
  EXPECT_EQ(curator.reassignStudentsWithDebts(2), 1U);
  EXPECT_TRUE(curator.transferToSubGroup("C1", 3));
  EXPECT_FALSE(curator.transferToSubGroup("C9", 3));
  EXPECT_EQ(curator.countSubGroup(3), 1U);
  EXPECT_TRUE(curator.removeStudent("C1"));
  EXPECT_FALSE(curator.removeStudent("C1"));
  EXPECT_THROW(curator.assignUnassignedStudentsToSubGroup(0), std::invalid_argument);
  EXPECT_THROW(curator.reassignStudentsWithDebts(0), std::invalid_argument);
  curator.printInfo();
}

TEST(AdditionalCoverage, DeanOfficeFullManagementPaths) {
  DeanOffice office("Dean office");
  EXPECT_EQ(office.name(), "Dean office");
  auto teacher = std::make_shared<Teacher>("Teacher", 40, "T1", "Math");
  auto curatorGroup = Group("CG", "CS", 5);
  auto curator = std::make_shared<Curator>("Curator", 45, "C1", curatorGroup);
  office.addWorker(teacher);
  office.addWorker(curator);
  EXPECT_EQ(office.totalWorkers(), 2U);
  EXPECT_EQ(office.findWorker("T1"), teacher.get());
  EXPECT_EQ(static_cast<const DeanOffice &>(office).findWorker("C1"), curator.get());
  EXPECT_THROW(office.addWorker(nullptr), std::invalid_argument);
  EXPECT_THROW(office.addWorker(teacher), std::invalid_argument);
  EXPECT_FALSE(office.removeWorker("missing"));
  EXPECT_TRUE(office.removeWorker("T1"));
  EXPECT_FALSE(office.assignCuratorToGroup("C1", "G1"));

  Specialty specialty("CS", "Computer Science", 4);
  office.registerSpecialty(specialty);
  EXPECT_EQ(office.specialtyCount(), 1U);
  EXPECT_NE(office.findSpecialty("CS"), nullptr);
  EXPECT_EQ(office.findSpecialty("NO"), nullptr);
  EXPECT_THROW(office.registerSpecialty(specialty), std::invalid_argument);
  EXPECT_FALSE(office.removeSpecialty("NO"));
  EXPECT_TRUE(office.removeSpecialty("CS"));

  office.addGroup(Group("G1", "CS", 5));
  EXPECT_TRUE(office.assignCuratorToGroup("C1", "G1"));
  office.addGroup(Group("G2", "CS", 5));
  office.addGroup(Group("G3", "OTHER", 5));
  office.addGroup(Group("EMPTY", "CS", 2));
  EXPECT_EQ(office.groupCount(), 4U);
  EXPECT_NE(office.findGroup("G1"), nullptr);
  EXPECT_EQ(office.findGroup("NO"), nullptr);
  EXPECT_TRUE(office.groupHasCapacity("G1"));
  EXPECT_FALSE(office.groupHasCapacity("NO"));
  EXPECT_TRUE(office.assignCuratorToGroup("C1", "G1"));
  EXPECT_FALSE(office.assignCuratorToGroup("NO", "G1"));

  Student student("Student", 20, "S1", 2);
  student.addMark(Mark(9, mathSubject(), Day("Mon", "2026-11-02")));
  EXPECT_TRUE(office.enrollStudent("G1", student));
  EXPECT_FALSE(office.enrollStudent("G1", student));
  EXPECT_FALSE(office.enrollStudent("NO", Student("Other", 20, "S2", 2)));
  EXPECT_EQ(office.totalStudents(), 1U);
  EXPECT_TRUE(office.hasStudent("S1"));
  EXPECT_NE(office.findStudent("S1"), nullptr);
  EXPECT_EQ(static_cast<const DeanOffice &>(office).findStudent("S1")->studentId(), "S1");
  EXPECT_EQ(office.groupOfStudent("S1"), "G1");
  EXPECT_EQ(office.groupOfStudent("S2"), "");
  EXPECT_EQ(office.promoteEligibleStudents("G1", 8.0), 1U);
  EXPECT_EQ(office.findStudent("S1")->year(), 3);
  EXPECT_THROW(office.promoteEligibleStudents("G1", 11.0), std::invalid_argument);
  EXPECT_EQ(office.promoteEligibleStudents("NO", 8.0), 0U);
  EXPECT_EQ(office.studentsWithAcademicDebts("G1"), 0U);
  EXPECT_EQ(office.studentsWithAcademicDebts("NO"), 0U);
  EXPECT_TRUE(office.transferStudent("G1", "G2", "S1"));
  EXPECT_FALSE(office.transferStudent("G1", "G2", "S1"));
  EXPECT_EQ(office.groupOfStudent("S1"), "G2");
  EXPECT_TRUE(office.removeFromGroup("G2", "S1"));
  EXPECT_TRUE(office.hasStudent("S1"));
  EXPECT_FALSE(office.removeFromGroup("G2", "S1"));
  EXPECT_TRUE(office.enrollStudent("G1", Student("Student2", 21, "S2", 2)));
  EXPECT_TRUE(office.expelStudent("S2"));
  EXPECT_FALSE(office.expelStudent("S2"));
  EXPECT_TRUE(office.removeEmptyGroup("EMPTY"));
  EXPECT_TRUE(office.removeEmptyGroup("G1"));
  EXPECT_FALSE(office.removeEmptyGroup("NO"));

  office.giveScholarship("S1");
  office.manipulateScholarship();
  EXPECT_FALSE(office.mergeGroups("G1", "G1"));
  EXPECT_FALSE(office.mergeGroups("G1", "G3"));
  office.addGroup(Group("M1", "CS", 3));
  office.addGroup(Group("M2", "CS", 5));
  EXPECT_TRUE(office.enrollStudent("M1", Student("Merge", 20, "SM", 2)));
  EXPECT_TRUE(office.mergeGroups("M1", "M2"));
  EXPECT_EQ(office.groupOfStudent("SM"), "M2");
  EXPECT_TRUE(office.mergeGroups("M1", "M2"));
}

TEST(AdditionalCoverage, StudentBenefitsScholarshipsAndApplicationBranches) {
  Student student("Scholarship Student", 20, "SS1", 1);
  EXPECT_EQ(student.studentId(), "SS1");
  EXPECT_EQ(student.year(), 1);
  EXPECT_EQ(student.role(), "student");
  EXPECT_TRUE(student.isEligibleForScholarship());
  student.awardScholarship();
  student.awardScholarship();
  student.removeScholarship();
  student.promoteToNextYear();
  EXPECT_EQ(student.year(), 2);
  student.setAcademicDebtCount(2);
  EXPECT_FALSE(student.isEligibleForScholarship());
  student.setAcademicDebtCount(-1);
  EXPECT_EQ(student.academicDebtCount(), 2);
  student.setAcademicDebtCount(0);

  Benefit benefit("Social", 250, "Support");
  student.assignBenefit(benefit);
  EXPECT_THROW(student.assignBenefit(benefit), std::invalid_argument);
  student.removeBenefit();
  EXPECT_THROW(student.removeBenefit(), std::invalid_argument);

  StudentApplication application("SS1", "Leave");
  student.submitApplication(application);
  EXPECT_EQ(student.applicationCount(), 1U);
  EXPECT_TRUE(student.reviewApplication("Leave"));
  EXPECT_FALSE(student.reviewApplication("Leave"));
  EXPECT_FALSE(student.reviewApplication("Missing"));
  EXPECT_THROW(student.submitApplication(StudentApplication("OTHER", "Bad")),
               std::invalid_argument);
  EXPECT_THROW(student.submitApplication(application), std::invalid_argument);

  ScholarshipApplication request("SS1", "Merit");
  student.requestScholarship(request);
  EXPECT_THROW(student.requestScholarship(request), std::invalid_argument);
  EXPECT_TRUE(student.resolveScholarshipApplication("Merit", false, "Low GPA"));
  EXPECT_FALSE(student.resolveScholarshipApplication("Merit", true));
  student.requestScholarship(request);
  EXPECT_TRUE(student.resolveScholarshipApplication("Merit", true));
  EXPECT_FALSE(student.resolveScholarshipApplication("Unknown", true));
  EXPECT_EQ(student.scholarshipApplicationCount(), 2U);
}

TEST(AdditionalCoverage, LessonAccessorsAndInvalidScheduleInputs) {
  EXPECT_THROW(Classroom("", 10), std::invalid_argument);
  EXPECT_THROW(Classroom("101", 0), std::invalid_argument);
  Classroom smallRoom("102", 12);
  EXPECT_TRUE(smallRoom.canFit(12));
  EXPECT_FALSE(smallRoom.canFit(13));
  EXPECT_THROW(smallRoom.canFit(-1), std::invalid_argument);

  Lecture lecture(mathSubject(), "T9", "Thursday", 8, 2, smallRoom);
  EXPECT_EQ(lecture.subject().code(), "MATH");
  EXPECT_EQ(lecture.teacherId(), "T9");
  EXPECT_EQ(lecture.day(), "Thursday");
  EXPECT_EQ(lecture.startHour(), 8);
  EXPECT_EQ(lecture.durationHours(), 2);
  Day day("Thursday", "2026-11-05");
  EXPECT_THROW(day.addLesson(nullptr), std::invalid_argument);
  day.addLesson(std::make_shared<Lecture>(lecture));
  EXPECT_THROW(day.hasTimeConflict(-1, 2), std::invalid_argument);
  EXPECT_THROW(day.hasTimeConflict(23, 2), std::invalid_argument);
  EXPECT_THROW(Schedule(""), std::invalid_argument);
  Week week(3);
  week.addDay(Day("Thursday", "2026-11-05"));
  EXPECT_THROW(week.addDay(Day("Another", "2026-11-05")), std::invalid_argument);
  Schedule schedule("G9");
  schedule.addWeek(week);
  EXPECT_THROW(schedule.addWeek(week), std::invalid_argument);
}

TEST(AdditionalCoverage, ConstructorValidationAndExceptionalBranches) {
  EXPECT_THROW(People("Bad", -1), std::invalid_argument);
  EXPECT_THROW(Worker("Bad", 20, "", "Position"), std::invalid_argument);
  EXPECT_THROW(Worker("Bad", 20, "W", ""), std::invalid_argument);
  EXPECT_THROW(DeanOffice(""), std::invalid_argument);
  EXPECT_THROW(AcademicOrder(0, "Order"), std::invalid_argument);
  EXPECT_THROW(AcademicOrder(1, ""), std::invalid_argument);
  EXPECT_THROW(TransferOrder(1, "", "A", "B"), std::invalid_argument);
  EXPECT_THROW(TransferOrder(1, "S", "", "B"), std::invalid_argument);
  EXPECT_THROW(ExpulsionOrder(1, "S", ""), std::invalid_argument);
  EXPECT_THROW(Assignment("", "Work"), std::invalid_argument);
  EXPECT_THROW(Assignment("S", ""), std::invalid_argument);
  EXPECT_THROW(Company("", "Name"), std::invalid_argument);
  EXPECT_THROW(Company("Type", ""), std::invalid_argument);
  EXPECT_THROW(FinancialReport(""), std::invalid_argument);
  EXPECT_THROW(Premy("", 1), std::invalid_argument);
  EXPECT_THROW(Premy("Award", -1), std::invalid_argument);
  EXPECT_THROW(Evaluation("", "Comment"), std::invalid_argument);
  EXPECT_THROW(StudentApplication("", "Topic"), std::invalid_argument);
  EXPECT_THROW(StudentApplication("S", ""), std::invalid_argument);
  EXPECT_THROW(StudyProgram("", 2026), std::invalid_argument);
  EXPECT_THROW(StudyProgram("Program", -1), std::invalid_argument);
  EXPECT_THROW(Specialty("", "Name", 4), std::invalid_argument);
  EXPECT_THROW(Specialty("CS", "Name", 0), std::invalid_argument);
  EXPECT_THROW(Curriculum("CS", 0), std::invalid_argument);
  EXPECT_THROW(Semester(1, 0), std::invalid_argument);
  EXPECT_THROW(Subject("", "Name", 1), std::invalid_argument);
  EXPECT_THROW(Subject("CS", "", 1), std::invalid_argument);
  EXPECT_THROW(Subject("CS", "Name", 0), std::invalid_argument);
  EXPECT_THROW(Classroom("101", 0), std::invalid_argument);
  EXPECT_THROW(Week(0), std::invalid_argument);
  EXPECT_THROW(Group("", "CS", 1), std::invalid_argument);
  EXPECT_THROW(Group("G", "CS", 0), std::invalid_argument);
  EXPECT_THROW(Schedule(""), std::invalid_argument);

  Dean dean("Dean", 50, "D", "Dean");
  AcademicOrder order(10, "Test");
  dean.approveOrder(order);
  EXPECT_TRUE(order.isApproved());

  Student student("Student", 20, "SX", 2);
  student.addMark(Mark(10, mathSubject(), Day("Mon", "2026-11-02")));
  EXPECT_TRUE(student.isEligibleForScholarship());
  student.awardScholarship();
  EXPECT_DOUBLE_EQ(student.averageMarkCheck(), 10.0);

  DeanOffice office("Office");
  EXPECT_THROW(office.giveScholarship("missing"), std::invalid_argument);
  office.addGroup(Group("DEBT", "CS", 2));
  Student debtor("Debtor", 20, "SD", 2);
  debtor.setAcademicDebtCount(1);
  EXPECT_TRUE(office.enrollStudent("DEBT", debtor));
  EXPECT_THROW(office.giveScholarship("SD"), std::invalid_argument);
  EXPECT_EQ(office.studentsWithAcademicDebts("DEBT"), 1U);
  office.manipulateScholarship();
  EXPECT_EQ(office.promoteEligibleStudents("DEBT", 0.0), 0U);
  EXPECT_FALSE(office.assignCuratorToGroup("missing", "DEBT"));
  EXPECT_FALSE(office.transferStudent("DEBT", "DEBT", "SD"));
  EXPECT_FALSE(office.transferStudent("missing", "DEBT", "SD"));

  Week fullWeek(7);
  for (int i = 0; i < 7; ++i) {
    fullWeek.addDay(Day("Day" + std::to_string(i),
                        "2026-12-" + std::to_string(10 + i)));
  }
  EXPECT_THROW(fullWeek.addDay(Day("Extra", "2026-12-20")), std::length_error);
}

TEST(AdditionalCoverage, PaymentAndAccountingAllOperations) {
  Student student("Student", 20, "S1", 2);
  AccountingDepartment accounting("Accounting");
  EXPECT_EQ(accounting.name(), "Accounting");
  PaymentRecord paid("S1", 100, "Tuition", "2026-11-01");
  EXPECT_DOUBLE_EQ(paid.amount(), 100);
  EXPECT_EQ(paid.purpose(), "Tuition");
  EXPECT_EQ(paid.date(), "2026-11-01");
  EXPECT_EQ(paid.studentId(), "S1");
  paid.markPaid();
  EXPECT_TRUE(paid.isPaid());
  accounting.recordPayment(paid);
  accounting.recordStudentPayment(student, 50, "Books", "2026-11-02");
  accounting.recordStudentPayment(student, 30, "Tuition", "2026-11-03");
  EXPECT_EQ(accounting.paymentCount(), 3U);
  EXPECT_EQ(accounting.paymentCountForStudent(student), 3U);
  EXPECT_DOUBLE_EQ(accounting.totalPaid(), 100);
  EXPECT_DOUBLE_EQ(accounting.totalPaidByStudent(student), 100);
  EXPECT_DOUBLE_EQ(accounting.totalPaidForStudent("S1"), 100);
  EXPECT_DOUBLE_EQ(accounting.totalUnpaid(), 80);
  EXPECT_TRUE(accounting.hasPayment("S1", "Books"));
  EXPECT_FALSE(accounting.hasPayment("S1", "Unknown"));
  EXPECT_EQ(accounting.paymentsForStudent("S1").size(), 3U);
  EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Tuition", false), 130);
  EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Tuition", true), 100);
  auto report = accounting.makeReport("November");
  EXPECT_EQ(report.period(), "November");
  EXPECT_DOUBLE_EQ(report.balance(), -100);
  EXPECT_EQ(accounting.settleUnpaidPaymentsForPurpose("S1", "Tuition"), 1U);
  EXPECT_EQ(accounting.markStudentPaymentsPaid("S1"), 1U);
  EXPECT_DOUBLE_EQ(accounting.totalUnpaid(), 0);
  accounting.recordStudentPayment(student, 25, "Penalty", "2026-11-04");
  EXPECT_EQ(accounting.cancelUnpaidPaymentsForPurpose("S1", "Penalty"), 1U);
  accounting.recordStudentPayment(student, 10, "Other", "2026-11-05");
  EXPECT_EQ(accounting.removeUnpaidPayments("S1"), 1U);
  EXPECT_EQ(accounting.paymentCount(), 3U);
}
