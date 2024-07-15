// g++ main.cpp -o main
// ./main

#include "base_class_linkedlist.cpp"
#include "base_class_tree.cpp"
#include "base_class_ui.cpp"

#include "data_tree_academic.cpp"

#include "data_class_students.cpp"
#include "data_class_courses.cpp"
#include "data_class_course_enrollments.cpp"
#include "data_class_course_grades.cpp"
#include "data_class_users.cpp"

#include "menu.cpp"

int main() {

    // init Tree (BEFORE init all other data becase nodes will be added during other data addNew)
    AcademicTree::init();

    // init data
    StudentList::init();
    CourseList::init();
    CourseEnrollmentList::init();
    CourseGradeList::init();
    UserList::init();

    // show menu
    Menu::showLogin();

    return 0;
}