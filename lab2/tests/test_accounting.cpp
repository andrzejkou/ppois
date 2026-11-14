#include <gtest/gtest.h>

#include "AccountingDepartment.h"
#include "PaymentRecord.h"
#include "Student.h"

#include <stdexcept>

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
