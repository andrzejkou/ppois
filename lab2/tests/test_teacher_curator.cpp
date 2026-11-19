#include <gtest/gtest.h>

#include "Teacher.h"
#include "Curator.h"
#include "Group.h"
#include "Student.h"
#include "Subject.h"

#include <stdexcept>

TEST(TeacherTest, ManagesCoursesAndSubjects) {
    Teacher teacher("Anna Petrova", 35, "T001", "Mathematics");
    Subject math("MATH", "Mathematics", 5);

    teacher.assignCourse("Algebra");
    EXPECT_EQ(teacher.courses().size(), 1U);
    EXPECT_TRUE(teacher.replaceCourse("Algebra", "Geometry"));
    EXPECT_FALSE(teacher.replaceCourse("Missing", "Calculus"));
    EXPECT_TRUE(teacher.removeCourse("Geometry"));

    teacher.assignSubject(math);
    EXPECT_TRUE(teacher.teachesSubject("MATH"));
    EXPECT_EQ(teacher.subjectCount(), 1U);
    EXPECT_TRUE(teacher.removeSubject("MATH"));
    EXPECT_FALSE(teacher.teachesSubject("MATH"));
}

TEST(TeacherTest, RejectsDuplicateAssignments) {
    Teacher teacher("Anna Petrova", 35, "T001", "Mathematics");
    teacher.assignCourse("Algebra");
    EXPECT_THROW(teacher.assignCourse("Algebra"), std::invalid_argument);
    EXPECT_THROW(teacher.assignCourse(""), std::invalid_argument);
}

TEST(CuratorTest, SupervisesAndReassignsStudents) {
    Group group("CS-21", "CS", 3);
    group += Student("Ivan Ivanov", 20, "S001", 2);
    group += Student("Petr Petrov", 21, "S002", 2);
    Curator curator("Sergey Sidorov", 40, "W001", group);

    EXPECT_EQ(curator.studentCount(), 2U);
    EXPECT_TRUE(curator.supervises("S001"));
    EXPECT_TRUE(curator.transferToSubGroup("S001", 1));
    EXPECT_EQ(curator.countSubGroup(1), 1U);
    EXPECT_FALSE(curator.transferToSubGroup("missing", 1));
}
