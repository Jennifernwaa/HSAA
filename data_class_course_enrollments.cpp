#pragma once

#include "base_class_linkedlist.cpp"
#include "base_class_ui.cpp"
#include "data_class_students.cpp"
#include "data_class_courses.cpp"

#include "data_tree_academic.cpp"

using namespace std;

// table_rows placeholder for showTable related (used in traverse in addTableRow)
// wrapped in IF because it's also defined in other data classes
#ifndef _TABLE_ROWS_FOR_DISPLAY
vector<vector<string> > table_rows; 
#define _TABLE_ROWS_FOR_DISPLAY
#endif

// Define a simple struct as an example of data
struct CourseEnrollment {
    string id; // for easier to remember strategy, should be a concat of student_id & course_id
    string student_id;
    string student_name; // will be auto populated in addNew or update if exists
    string course_id;
    string course_name; // will be auto populated in addNew or update if exists
 
};

LinkedList<CourseEnrollment> course_enrollment_list;
   
class CourseEnrollmentList {

public:

    // add data for quick test
    static void init() {

        // Create initial data nodes
        // node id should be unique, so it's better to combine student id & course id (separated by colon for readability)

        addNew("S001:CMAT10", "S001", "CMAT10");
        // addNew("S001:CBIO10", "S001", "CBIO10");
        addNew("S001:CPHY10", "S001", "CPHY10");
        // addNew("S001:CCHE10", "S001", "CCHE10");
    
        addNew("S002:CMAT10", "S002", "CMAT10");
        // addNew("S002:CBIO10", "S002", "CBIO10");
        // addNew("S002:CPHY10", "S002", "CPHY10");
        addNew("S002:CCHE10", "S002", "CCHE10");
    
        // addNew("S004:CMAT11", "S004", "CMAT11");
        addNew("S004:CBIO11", "S004", "CBIO11");
        addNew("S004:CPHY11", "S004", "CPHY11");
        // addNew("S004:CCHE11", "S004", "CCHE11");
    
        // addNew("S005:CMAT11", "S005", "CMAT11");
        // addNew("S005:CBIO11", "S005", "CBIO11");
        addNew("S005:CPHY11", "S005", "CPHY11");
        addNew("S005:CCHE11", "S005", "CCHE11");

        // addNew("S007CMAT12", "S007", "CMAT12");
        // addNew("S007CBIO12", "S007", "CBIO12");
        addNew("S007:CPHY12", "S007", "CPHY12");
        addNew("S007:CCHE12", "S007", "CCHE12");

        addNew("S008:CMAT12", "S008", "CMAT12");
        addNew("S008:CBIO12", "S008", "CBIO12");
        // addNew("S008CPHY12", "S008", "CPHY12");
        // addNew("S008CCHE12", "S008", "CCHE12");
    }

    // add new 
    static bool addNew (const string& id, const string& student_id, const string& course_id ) {
        bool exists = course_enrollment_list.nodeExists (id);

        if (!exists) {
         // get student & course data to populate names            
            Student student_data = StudentList::getData(student_id);
            Course course_data = CourseList::getData(course_id);

            CourseEnrollment data = {id, student_id, student_data.name, course_id, course_data.name};
            course_enrollment_list.addNode (id, data);
         
            string info = student_data.name + " / " + course_data.name;

            // should add course to tree (under student)
            AcademicTree::addCourseEnrollment (student_id, id, info);
            
            return true;
        } else {
            return false; // node with the same id already exists
        }
    }

    // update :: enrollment does NOT need an update, just remove and addNew if you made a mistaken enrollment

    // delete existing
    static bool remove (const string& id) {
        bool exists = course_enrollment_list.nodeExists (id);

        if (exists) {
            course_enrollment_list.deleteNode (id);

            // also remove from tree
            AcademicTree::removeCourseEnrollment(id);
            return true;
        } else {
            return false; // node with that id does not exists
        }
    }

    // exists
    static bool exists (const string& id) {
        return course_enrollment_list.nodeExists (id);
    }

    // get data
    static CourseEnrollment getData (const string& id) {
        bool exists = course_enrollment_list.nodeExists (id);
        Node<CourseEnrollment>* node;
        CourseEnrollment data;
        if (exists) {
            node = course_enrollment_list.findNode(id);
            data = node->data;
        }
        return data;
    }
    
    // display record
    static void showRecord  (const string& id) {
        bool exists = course_enrollment_list.nodeExists (id);

        if (exists) {
            CourseEnrollment data = getData(id);
            int padding = 15;

            UI::showLine ( UI::strPadEnd ( "Id", padding) + ": " + data.id );
            UI::showLine ( UI::strPadEnd ( "Student Id", padding) + ": " + data.student_id );
            UI::showLine ( UI::strPadEnd ( "Student Name", padding) + ": " + data.student_name );
            UI::showLine ( UI::strPadEnd ( "Course Id", padding) + ": " + data.course_id );
            UI::showLine ( UI::strPadEnd ( "Course Name", padding) + ": " + data.course_name );
            
        } else {
            UI::showLine ("Not found.");
        }
    }

    // showTable related
    static void tableInitRows() {
        table_rows.clear();   
    }

    static void tableAddRow (Node<CourseEnrollment>* node ) {
        CourseEnrollment data = node->data;

        vector<string> row;
        row.push_back (data.id);
        row.push_back (data.student_id);
        row.push_back (data.student_name);
        row.push_back (data.course_id);
        row.push_back (data.course_name);

        table_rows.push_back(row); // add to rows
    }

    static void showTable(const string& title) {
        tableInitRows();

        // use anonymous callback function to build rows
        // add row one by one per node
        course_enrollment_list.traverse ( tableAddRow );

        const string headers[] = {"Id", "Student Id", "Student Name", "Course Id", "Course Name"};
        int col_sizes[] = {15, 10, 20, 10, 20};
        int num_cols = 5;
    
        // exit(EXIT_SUCCESS);

        UI::showEmptyLine(1);
        UI::showTable(title, headers, table_rows, col_sizes, num_cols);
        UI::showEmptyLine(1);
    }
    
};