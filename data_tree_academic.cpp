#pragma once

#include "base_class_tree.cpp"
#include "base_class_ui.cpp"

using namespace std;

#ifndef _ACADEMIC_TREE
TreeNode* academic_root = new TreeNode("root", "root");
#define _ACADEMIC_TREE
#endif
  
// Callback function to display nodes with indentation
void displayNodeDebug(TreeNode* TreeNode, int level) {
    // ignore root, level = 0
    if (level > 0) {
        for (int i = 1; i < level; i++) {
            cout << "   "; // Two spaces for each level of indentation
        }
        cout << "L:" << level << "  T:" << TreeNode->type << "  ID:" << TreeNode->id << "  DATA:" << TreeNode->data << endl;
    }
}

void displayNode(TreeNode* TreeNode, int level) {
    // ignore root, level = 0
    if (level > 0) {
        for (int i = 1; i < level; i++) {
            cout << "   "; // Two spaces for each level of indentation
        }

        if (TreeNode->type == "grade_level") {
            cout << endl << TreeNode->data << endl;

        } else if (TreeNode->type == "student") {
            cout << TreeNode->data << endl;

        } else if (TreeNode->type == "course_grade") {
            string course_grade_id =  UI::strReplace(TreeNode->id , ":grade", ""); // make it shown as orginal course_grade_id (as ":grade" was added only to make a unique tree node id)
            cout << course_grade_id << " - " << TreeNode->data << endl;

        } else {
            cout << TreeNode->id << " - " << TreeNode->data << endl;
        }
    }
}

class AcademicTree {

public:

    // add data for quick test
    static void init() {

        // initialize Tree structure: Grade Level -> Student -> Course -> Grade
        TreeNode* gradeG10 = new TreeNode("grade_level", "G10", "Grade 10");
        TreeNode* gradeG11 = new TreeNode("grade_level", "G11", "Grade 11");
        TreeNode* gradeG12 = new TreeNode("grade_level", "G12", "Grade 12");
    
        academic_root->addChild(gradeG10);
        academic_root->addChild(gradeG11);
        academic_root->addChild(gradeG12);
    }

    // showTree
    static void showTree(TreeNode* start_node = academic_root) {
        UI::showLine ("Academic Summary Information (Tree)");
        // Traverse the tree and display nodes with indentation using the callback function
        Tree::traverse(start_node, displayNode);
        UI::showEmptyLine();
    }

    // showTreeDebug
    static void showTreeDebug(TreeNode* start_node = academic_root) {
        UI::showLine ("DEBUG :: Academic Summary Information (Tree)");
        // Traverse the tree and display nodes with indentation using the callback function
        Tree::traverse(start_node, displayNodeDebug);
        UI::showEmptyLine();
    }

    // showStudentTree
    static void showStudentTree(const string& id) {
        TreeNode* start_node = Tree::findNode(academic_root, "student", id);
        if (start_node != nullptr) {
            UI::showLine ("Student Academic Summary Information (Tree)");
            UI::showEmptyLine(1);
            Tree::traverse(start_node, displayNode);
            UI::showEmptyLine();

        } else {
            UI::showEmptyLine(1);
            UI::showLine ("Student with ID: " + id + " is not found");
        }
    }

    // tree node operations
    
    // ASSUMPTIONS: when doing tree operations it is ASSUMMED that the data passed are VALID 
    // because usually it's called from data (LinkedList addNew or remove methods that are already validated)

    // add tree node (generic method)
    static void addChildToParentNode ( const string& p_node_type, const string& p_node_id,  const string& c_node_type, const string& c_node_id, const string& c_data="" ) {
        // find parent node
        TreeNode* p_node = Tree::findNode (academic_root, p_node_type, p_node_id);

        // attach child node
        TreeNode* c_node = new TreeNode(c_node_type, c_node_id, c_data);
        p_node->addChild (c_node);
    }

    // STUDENTS
    // addStudent
    static void addStudent (const int& grade_level, const string& student_id, const string& data="") {
        // parent to find
        string p_node_type = "grade_level";
        string p_node_id = "G" + to_string(grade_level); 

        // child to attach
        string c_node_type = "student";
        string c_node_id = student_id;

        // attach
        addChildToParentNode ( p_node_type, p_node_id, c_node_type, c_node_id, data );
    }
    
    // update: pass data as string
    static void updateStudent (const int& grade_level, const string& student_id, const string& data="") {
        // child to find
        string c_node_type = "student";
        string c_node_id = student_id;

        TreeNode* c_node = Tree::findNode (academic_root, c_node_type, c_node_id);
        c_node->data = data;
    }

    // removeStudent
    static void removeStudent ( const string& student_id ) {
        Tree::removeNode (academic_root, "student", student_id );
    }

    // COURSE ENROLLMENTS
    // addCourseEnrollment: course_enrollment_id = student_id + ":" + course_id (see data_class_course_enrollments)
    static void addCourseEnrollment (const string& student_id, const string& course_enrollment_id, const string& data="") {
        // parent to find
        string p_node_type = "student";
        string p_node_id = student_id; 

        // child to attach
        string c_node_type = "course_enrollment";
        string c_node_id = course_enrollment_id;

        // attach
        addChildToParentNode ( p_node_type, p_node_id, c_node_type, c_node_id, data );
    }

    // removeCourseEnrollment
    static void removeCourseEnrollment ( const string& course_enrollment_id ) {
        Tree::removeNode (academic_root, "course_enrollment", course_enrollment_id );
    }
    
    // COURSE GRADE
    // addCourseGrade
    static void addCourseGrade (const string& course_grade_id, const string& data="") {
        // course_grade_id is the same as course_enrollment_id
        string course_enrollment_id = course_grade_id;

        // parent to find
        string p_node_type = "course_enrollment";
        string p_node_id = course_enrollment_id; 

        // child to attach
        string c_node_type = "course_grade";
        string c_node_id = course_grade_id + ":grade"; // make it unique for a tree node (cannot have the exact same node id as enrollment)

        // attach
        addChildToParentNode ( p_node_type, p_node_id, c_node_type, c_node_id, data );
    }

    // updateCourseGrade
    static void updateCourseGrade (const string& course_grade_id, const string& data="") {
        // node to update
        string c_node_type = "course_grade";
        string c_node_id = course_grade_id + ":grade"; // make it unique for a tree node (cannot have the exact same node id as enrollment)
        
        TreeNode* c_node = Tree::findNode (academic_root, c_node_type, c_node_id);
        c_node->data = data;
    }

    // removeCourseGrade
    static void removeCourseGrade ( const string& course_grade_id ) {
        Tree::removeNode (academic_root, "course_grade", course_grade_id + ":grade" );
    }
    
    
};