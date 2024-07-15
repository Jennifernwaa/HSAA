#pragma once

#include "base_class_linkedlist.cpp"
#include "base_class_ui.cpp"
#include "data_class_students.cpp"
#include "data_class_courses.cpp"
#include "data_class_course_enrollments.cpp"

#include "data_tree_academic.cpp"

using namespace std;

// table_rows placeholder for showTable related (used in traverse in addTableRow)
// wrapped in IF because it's also defined in other data classes
#ifndef _TABLE_ROWS_FOR_DISPLAY
vector<vector<string> > table_rows; 
#define _TABLE_ROWS_FOR_DISPLAY
#endif

// Define a simple struct as an example of data
struct CourseGrade {
    string id; // id of the node (should be == course_enrollment_id)
    string grade;
    // added for table display
    string student_id; // will be auto populated in addNew or update if exists
    string student_name; // will be auto populated in addNew or update if exists
    string course_id; // will be auto populated in addNew or update if exists
    string course_name; // will be auto populated in addNew or update if exists
};

LinkedList<CourseGrade> course_grade_list;
   
class CourseGradeList {

public:

    // add data for quick test
    static void init() {

        // Create initial data nodes
        // node id should be unique, for readability

        addNew("S001:CMAT10", "A");
        addNew("S001:CBIO10", "C");
        addNew("S001:CPHY10", "B");
        addNew("S001:CCHE10", "D");
    
        addNew("S002:CMAT10", "B");
        addNew("S002:CBIO10", "A");
        addNew("S002:CPHY10", "C");
        addNew("S002:CCHE10", "A");
    
       // addNew("S004:CMAT11", "C");
        addNew("S004:CBIO11", "B");
        addNew("S004:CPHY11", "A");
       // addNew("S004:CCHE11", "D");
    
        addNew("S005:CMAT11", "A");
        // addNew("S005:CBIO11", "B");
        addNew("S005:CPHY11", "A");
        addNew("S005:CCHE11", "C");

        // addNew("S007CMAT12", "B");
        // addNew("S007CBIO12", "D");
        addNew("S007:CPHY12", "C");
       addNew("S007:CCHE12", "A");

        addNew("S008:CMAT12", "C");
        addNew("S008:CBIO12", "A");
        // addNew("S008CPHY12", "B");
       // addNew("S008CCHE12", "D");
    }

    // add new 
    static bool addNew (const string& id, const string& grade ) {
        bool exists = course_grade_list.nodeExists (id);

        bool enrollment_exists = CourseEnrollmentList::exists(id);
        
        // only add if the id doesnt exists yet BUT it has valid course enrollment id
        if (!exists && enrollment_exists) {

            // get student & course data to populate names            
            CourseEnrollment data= CourseEnrollmentList::getData(id);
        
            CourseGrade grade_data = {id, grade, data.student_id, data.student_name, data.course_id, data.course_name};
            course_grade_list.addNode (id, grade_data);

            // string info = data.student_name + " / " + data.course_name + " = " + grade;
            string info = "Course Grade = " + grade;

            // should add course to tree (under student->course)
            AcademicTree::addCourseGrade (id, info);
            
            return true;
        } else {
            return false; // node with the same id already exists
        }
    }

    // update: can only update grade 
    // must never update id, if you need to change id, then remove and then addNew
    static bool update (const string& id, const string& grade ) {
        bool exists = course_grade_list.nodeExists (id);

        if (!exists) {
            return false; // node does not exists
        } else {
            CourseGrade old_data = getData(id);
            CourseGrade updated_data = old_data;
            updated_data.grade = grade;
            
            Node<CourseGrade>* node;
            node = course_grade_list.findNode(id);
            node->data = updated_data;
      
            string info = "Course Grade = " + grade;

            // should update grade in tree (under enrollment id), since course grade id = enrollment id, it can be used to update the grade node 
            AcademicTree::updateCourseGrade (id, info);
            return true;
        }
    }

    // delete existing
    static bool remove (const string& id) {
        bool exists = course_grade_list.nodeExists (id);

        if (exists) {
            course_grade_list.deleteNode (id);
            
            // also remove from tree
            AcademicTree::removeCourseGrade(id);
            return true;
            
        } else {
            return false; // node with that id does not exists
        }
    }

    // exists
    static bool exists (const string& id) {
        return course_grade_list.nodeExists (id);
    }

    // get data
    static CourseGrade getData (const string& id) {
        bool exists = course_grade_list.nodeExists (id);
        Node<CourseGrade>* node;
        CourseGrade data;
        if (exists) {
            node = course_grade_list.findNode(id);
            data = node->data;
        }
        return data;
    }

    // display record
    static void showRecord  (const string& id) {
        bool exists = course_grade_list.nodeExists (id);

        if (exists) {
            CourseGrade data = getData(id);
            int padding = 15;

            UI::showLine ( UI::strPadEnd ( "Id", padding) + ": " + data.id );
            UI::showLine ( UI::strPadEnd ( "Student Id", padding) + ": " + data.student_id );
            UI::showLine ( UI::strPadEnd ( "Student Name", padding) + ": " + data.student_name );
            UI::showLine ( UI::strPadEnd ( "Course Id", padding) + ": " + data.course_id );
            UI::showLine ( UI::strPadEnd ( "Course Name", padding) + ": " + data.course_name );
            UI::showLine ( UI::strPadEnd ( "Course Grade", padding) + ": " + data.grade );
            
        } else {
            UI::showLine ("Not found.");
        }
    }

    // showTable related
    static void tableInitRows() {
        table_rows.clear();   
    }

    static void tableAddRow (Node<CourseGrade>* node ) {
        CourseGrade data = node->data;

        vector<string> row;
        row.push_back (data.id);
        row.push_back (data.student_id);
        row.push_back (data.student_name);
        row.push_back (data.course_id);
        row.push_back (data.course_name);
        row.push_back (data.grade);       

        table_rows.push_back(row); // add to rows
    }

    static void showTable(const string& title) {
        tableInitRows();

        // add row one by one per node
        course_grade_list.traverse ( tableAddRow );

        const string headers[] = {"Id", "Student Id", "Student Name", "Course Id", "Course Name", "Grade"};
        int col_sizes[] = {15, 10, 20, 10, 20, 5};
        int num_cols = 6;
    
        // exit(EXIT_SUCCESS);

        UI::showEmptyLine(1);
        UI::showTable(title, headers, table_rows, col_sizes, num_cols);
        UI::showEmptyLine(1);
    }
    
};