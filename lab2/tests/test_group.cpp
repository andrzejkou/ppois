#include <gtest/gtest.h>

#include "Group.h"
#include "Student.h"
#include "exceptions.h"

#include <memory>
#include <stdexcept>

TEST(GroupTest, AddsFindsAndRemovesStudents) {
    Group group("CS-21", "CS", 2);
    Student first("Ivan Ivanov", 20, "S001", 2);
    Student second("Petr Petrov", 21, "S002", 2);

    group += first;
    group += std::make_shared<Student>(second);

    EXPECT_EQ(group.size(), 2U);
    EXPECT_TRUE(group.containsStudent("S001"));
    ASSERT_NE(group.findStudent("S002"), nullptr);
    EXPECT_FALSE(group.hasCapacity());
    EXPECT_EQ(group.availableSeats(), 0U);
    EXPECT_TRUE(group -= "S001");
    EXPECT_FALSE(group -= "missing");
}

TEST(GroupTest, RejectsDuplicatesAndCapacityOverflow) {
    Group group("CS-21", "CS", 1);
    Student student("Ivan Ivanov", 20, "S001", 2);
    group += student;

    EXPECT_THROW(group += student, DuplicateStudentException);
    EXPECT_THROW(group += Student("Petr Petrov", 21, "S002", 2),
                 GroupCapacityException);
}

TEST(GroupTest, AssignsAndFiltersSubgroups) {
    Group group("CS-21", "CS", 3);
    group += Student("Ivan Ivanov", 20, "S001", 2);
    group += Student("Petr Petrov", 21, "S002", 2);

    EXPECT_EQ(group.unassignedStudentCount(), 2U);
    EXPECT_EQ(group.assignSubGroup(1), 2U);
    EXPECT_EQ(group.studentsInSubGroup(1).size(), 2U);
    EXPECT_EQ(group.unassignedStudentCount(), 0U);
    EXPECT_THROW(group.assignSubGroup(0), std::invalid_argument);
}
