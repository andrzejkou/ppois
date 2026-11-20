#include <gtest/gtest.h>

#include "AcademicOrder.h"
#include "AcademicWork.h"
#include "AccountingDepartment.h"
#include "Curator.h"
#include "Dean.h"
#include "FinancialReport.h"
#include "Group.h"
#include "PaymentRecord.h"
#include "Schedule.h"
#include "Secretary.h"
#include "Student.h"
#include "Submission.h"
#include "Teacher.h"

TEST(ModelMetadata, PersonAndStudentMetadataCanBeStored) {
  Student student("Ivan Ivanov", 20, "S-100", 2);
  student.setEmail("ivan@example.com");
  student.setPhone("+70000000000");
  student.setEnrollmentDate("2026-09-01");
  student.setEducationForm("очная");
  student.setFundingType("бюджет");

  EXPECT_EQ(student.email(), "ivan@example.com");
  EXPECT_EQ(student.phone(), "+70000000000");
  EXPECT_EQ(student.enrollmentDate(), "2026-09-01");
  EXPECT_EQ(student.educationForm(), "очная");
  EXPECT_EQ(student.fundingType(), "бюджет");
  EXPECT_EQ(student.status(), "обучается");
}

TEST(ModelMetadata, GroupAndCuratorMetadataAreAvailable) {
  Group group("CS-21", "CS", 25);
  group.setAdmissionYear(2026);
  group.setEducationForm("очная");
  group.setCuratorEmployeeId("CUR-1");

  EXPECT_EQ(group.admissionYear(), 2026);
  EXPECT_EQ(group.educationForm(), "очная");
  EXPECT_EQ(group.curatorEmployeeId(), "CUR-1");

  Curator curator("Anna Petrova", 40, "CUR-1", group);
  curator.setAppointmentDate("2026-09-01");
  curator.setConsultationSchedule("Пятница, 14:00-16:00");
  curator.setOfficeNumber("314");
  EXPECT_EQ(curator.appointmentDate(), "2026-09-01");
  EXPECT_EQ(curator.consultationSchedule(), "Пятница, 14:00-16:00");
  EXPECT_EQ(curator.officeNumber(), "314");
}

TEST(ModelMetadata, AcademicAndFinancialMetadataAreAvailable) {
  AcademicOrder order(1, "Зачисление");
  order.setIssueDate("2026-09-01");
  order.setEffectiveDate("2026-09-01");
  order.setApproverEmployeeId("DEAN-1");
  EXPECT_EQ(order.issueDate(), "2026-09-01");
  EXPECT_EQ(order.effectiveDate(), "2026-09-01");
  EXPECT_EQ(order.approverEmployeeId(), "DEAN-1");

  PaymentRecord payment("S-100", 1000.0, "Обучение", "2026-09-01");
  payment.setDueDate("2026-09-10");
  payment.setPaymentMethod("Перевод");
  payment.setReceiptNumber("R-100");
  EXPECT_EQ(payment.dueDate(), "2026-09-10");
  EXPECT_EQ(payment.paymentMethod(), "Перевод");
  EXPECT_EQ(payment.receiptNumber(), "R-100");

  FinancialReport report("2026-09");
  report.setPeriodStart("2026-09-01");
  report.setPeriodEnd("2026-09-30");
  EXPECT_EQ(report.periodStart(), "2026-09-01");
  EXPECT_EQ(report.periodEnd(), "2026-09-30");
}
