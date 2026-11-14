#include <gtest/gtest.h>

#include "DeanOffice.h"
#include "Student.h"
#include "Teacher.h"

#include <memory>
#include <stdexcept>

TEST(DeanOfficeTest, EnrollsTransfersAndExpelsStudents) {
    DeanOffice office("Dean office");
    office.addGroup(Group("CS-21", "CS", 3));
    office.addGroup(Group("CS-22", "CS", 3));
    Student student("Ivan Ivanov", 20, "S001", 2);

    ASSERT_TRUE(office.enrollStudent("CS-21", student));
    EXPECT_FALSE(office.enrollStudent("CS-21", student));
    EXPECT_EQ(office.totalStudents(), 1U);
    EXPECT_EQ(office.groupOfStudent("S001"), "CS-21");
    EXPECT_TRUE(office.transferStudent("CS-21", "CS-22", "S001"));
    EXPECT_EQ(office.groupOfStudent("S001"), "CS-22");
    EXPECT_TRUE(office.expelStudent("S001"));
    EXPECT_FALSE(office.hasStudent("S001"));
}

TEST(DeanOfficeTest, ManagesWorkers) {
    DeanOffice office("Dean office");
    auto teacher = std::make_shared<Teacher>("Anna Petrova", 35, "T001", "Math");

    office.addWorker(teacher);
    EXPECT_EQ(office.totalWorkers(), 1U);
    ASSERT_NE(office.findWorker("T001"), nullptr);
    EXPECT_EQ(office.findWorker("T001")->employeeId(), "T001");
    EXPECT_TRUE(office.removeWorker("T001"));
    EXPECT_FALSE(office.removeWorker("T001"));
}

TEST(DeanOfficeTest, RejectsInvalidGroupsAndMissingTransfers) {
    DeanOffice office("Dean office");
    office.addGroup(Group("CS-21", "CS", 1));
    EXPECT_FALSE(office.transferStudent("CS-21", "UNKNOWN", "S001"));
    EXPECT_FALSE(office.removeEmptyGroup("UNKNOWN"));
}
