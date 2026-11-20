#include <gtest/gtest.h>

#include "AccountingDepartment.h"
#include "PaymentRecord.h"
#include "Student.h"

#include <stdexcept>
#include <limits>

TEST(PaymentRecordTest, ValidatesAndMarksPayment) {
    PaymentRecord payment("S001", 1500.0, "Tuition", "2026-11-14");
    EXPECT_FALSE(payment.isPaid());
    payment.markPaid();
    EXPECT_TRUE(payment.isPaid());
    EXPECT_DOUBLE_EQ(payment.amount(), 1500.0);
    EXPECT_THROW(PaymentRecord("", 100.0, "Tuition", "2026-11-14"),
                 std::invalid_argument);
    EXPECT_THROW(PaymentRecord("S001", 0.0, "Tuition", "2026-11-14"),
                 std::invalid_argument);
}

TEST(AccountingDepartmentTest, CalculatesAndSettlesPayments) {
    AccountingDepartment accounting("Accounting");
    Student student("Ivan Ivanov", 20, "S001", 2);
    accounting.recordStudentPayment(student, 1000.0, "Tuition", "2026-11-01");
    accounting.recordStudentPayment(student, 500.0, "Books", "2026-11-02");

    EXPECT_EQ(accounting.paymentCountForStudent(student), 2U);
    EXPECT_DOUBLE_EQ(accounting.totalUnpaid(), 1500.0);
    EXPECT_TRUE(accounting.hasPayment("S001", "Tuition"));
    EXPECT_EQ(accounting.settleUnpaidPaymentsForPurpose("S001", "Tuition"), 1U);
    EXPECT_DOUBLE_EQ(accounting.totalPaidByStudent(student), 1000.0);
    EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Books", false), 500.0);
    EXPECT_EQ(accounting.removeUnpaidPayments("S001"), 1U);
    EXPECT_EQ(accounting.paymentCount(), 1U);
}

TEST(AccountingDepartmentTest, FiltersAndCancelsPayments) {
    AccountingDepartment accounting("Accounting");
    accounting.recordPayment(PaymentRecord("S001", 200.0, "Dormitory", "2026-11-01"));
    accounting.recordPayment(PaymentRecord("S002", 300.0, "Dormitory", "2026-11-01"));

    EXPECT_EQ(accounting.paymentsForStudent("S001").size(), 1U);
    EXPECT_DOUBLE_EQ(accounting.totalForPurpose("Dormitory", false), 500.0);
    EXPECT_EQ(accounting.cancelUnpaidPaymentsForPurpose("S001", "Dormitory"), 1U);
    EXPECT_FALSE(accounting.hasPayment("S001", "Dormitory"));
}


TEST(PaymentRecordTest, ChangesAmountAndPurposeWithValidation) {
    PaymentRecord payment("S001", 1500.0, "Tuition", "2026-11-14");

    payment.changeAmount(2000.0);
    payment.changePurpose("Dormitory");

    EXPECT_DOUBLE_EQ(payment.amount(), 2000.0);
    EXPECT_EQ(payment.purpose(), "Dormitory");

    EXPECT_THROW(payment.changeAmount(0.0), std::invalid_argument);
    EXPECT_THROW(payment.changeAmount(-1.0), std::invalid_argument);
    EXPECT_THROW(payment.changeAmount(std::numeric_limits<double>::quiet_NaN()),
                 std::invalid_argument);
    EXPECT_THROW(payment.changeAmount(std::numeric_limits<double>::infinity()),
                 std::invalid_argument);
    EXPECT_THROW(payment.changePurpose(""), std::invalid_argument);

    // Failed updates must not corrupt the previously stored values.
    EXPECT_DOUBLE_EQ(payment.amount(), 2000.0);
    EXPECT_EQ(payment.purpose(), "Dormitory");
}

TEST(AccountingDepartmentTest, ReportsOnlyPaidPaymentsForRequestedCalendarPeriod) {
    AccountingDepartment accounting("Accounting");

    PaymentRecord november14("S001", 100.0, "Tuition", "2026-11-14");
    november14.markPaid();
    accounting.recordPayment(november14);

    PaymentRecord november15("S001", 50.0, "Books", "2026-11-15");
    november15.markPaid();
    accounting.recordPayment(november15);

    PaymentRecord december("S001", 70.0, "Dormitory", "2026-12-01");
    december.markPaid();
    accounting.recordPayment(december);

    PaymentRecord previousYear("S002", 30.0, "Tuition", "2025-11-14");
    previousYear.markPaid();
    accounting.recordPayment(previousYear);

    // Unpaid payments must not be included in a financial report.
    accounting.recordPayment(
        PaymentRecord("S001", 999.0, "Tuition", "2026-11-14"));

    EXPECT_DOUBLE_EQ(accounting.makeReport("2026").balance(), -220.0);
    EXPECT_DOUBLE_EQ(accounting.makeReport("2026-11").balance(), -150.0);
    EXPECT_DOUBLE_EQ(accounting.makeReport("2026-11-14").balance(), -100.0);
    EXPECT_DOUBLE_EQ(accounting.makeReport("November").balance(), -180.0);
    EXPECT_DOUBLE_EQ(accounting.makeReport("nOvEmBeR").balance(), -180.0);

    // Descriptive/unsupported periods retain the historical all-period behavior.
    EXPECT_DOUBLE_EQ(accounting.makeReport("all periods").balance(), -250.0);
    EXPECT_DOUBLE_EQ(accounting.makeReport("20A6").balance(), -250.0);
    EXPECT_DOUBLE_EQ(accounting.makeReport("2026-13").balance(), -250.0);

    EXPECT_EQ(accounting.makeReport("2026-11").period(), "2026-11");
}

TEST(AccountingDepartmentTest, PaymentLookupReturnsOnlyMatchingStudentRecords) {
    AccountingDepartment accounting("Accounting");
    accounting.recordPayment(
        PaymentRecord("S001", 100.0, "Tuition", "2026-11-01"));
    accounting.recordPayment(
        PaymentRecord("S002", 200.0, "Books", "2026-11-02"));
    accounting.recordPayment(
        PaymentRecord("S001", 300.0, "Dormitory", "2026-11-03"));

    const auto studentPayments = accounting.paymentsForStudent("S001");
    ASSERT_EQ(studentPayments.size(), 2U);
    EXPECT_EQ(studentPayments[0].studentId(), "S001");
    EXPECT_EQ(studentPayments[1].studentId(), "S001");
    EXPECT_TRUE(accounting.paymentsForStudent("missing").empty());
}
