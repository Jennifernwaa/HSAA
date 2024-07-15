#pragma once

#include "base_class_linkedlist.cpp"
#include "base_class_ui.cpp"

#include "data_tree_academic.cpp"

using namespace std;

// table_rows placeholder for showTable related (used in traverse in addTableRow)
// wrapped in IF because it's also defined in other data classes
#ifndef _TABLE_ROWS_FOR_DISPLAY
vector<vector<string> > table_rows; 
#define _TABLE_ROWS_FOR_DISPLAY
#endif


// Define a simple struct as an example of data
struct Student {
    string id;
    string name;
    int grade_level;
    string city_of_birth;
};

LinkedList<Student> student_list;
   
class StudentList {

public:

    // add data for quick test
    static void init() {

        // Create initial data nodes
        addNew("S001", "Alice", 10, "Surabaya");
        addNew("S002", "Charlie", 10, "Semarang");
        addNew("S003", "Hansen", 10, "Jakarta");
        
        addNew("S004", "Brandon", 11, "Yogyakarta");
        addNew("S005", "David", 11, "Surakarta");
        addNew("S006", "James", 11, "Malang");

        addNew("S007", "Francis", 12, "Denpasar");
        addNew("S008", "George", 12, "Bogor");
        addNew("S009", "Kaleb", 12, "Bandung");
    }

    // exists
    static bool exists (const string& id) {
        return student_list.nodeExists (id);
    }

    // get data
    static Student getData (const string& id) {
        bool exists = student_list.nodeExists (id);
        Node<Student>* node;
        Student data;
        if (exists) {
            node = student_list.findNode(id);
            data = node->data;
        }
        return data;
    }

    // add new 
    static bool addNew (const string& id, const string& name, const int& grade_level, const string& city_of_birth ) {
        bool exists = student_list.nodeExists (id);

        if (!exists) {
            Student data = { id, name, grade_level, city_of_birth };
            student_list.addNode (id, data);

            string info = id + " " + name + " / " + to_string(grade_level) + " / " + city_of_birth; 

            // should add student to tree (under grade level)
            AcademicTree::addStudent (grade_level, id, info);

            return true;
        } else {
            return false; // node with the same id already exists
        }
    }

    // update: can only update name and city_of_birth
    // must never update id, if you need to change id or grade_level, then remove and then addNew
    static bool update (const string& id, const string& name, const string& city_of_birth ) {
        bool exists = student_list.nodeExists (id);

        if (!exists) {
            return false; // node does not exists
        } else {
            Student old_data = getData(id);
            Student updated_data = { id, name, old_data.grade_level, city_of_birth };
            
            Node<Student>* node;
            node = student_list.findNode(id);
            node->data = updated_data;
      
            string info = id + " " + name + " / " + to_string(old_data.grade_level) + " / " + city_of_birth; 

            AcademicTree::updateStudent (old_data.grade_level, id, info);

            return true;
        }
    }

    // delete existing
    static bool remove (const string& id) {
        bool exists = student_list.nodeExists (id);

        if (exists) {
            student_list.deleteNode (id);

            // also remove from tree
            AcademicTree::removeStudent(id);
            return true;
        } else {
            return false; // node with that id does not exists
        }
    }

    // display record
    static void showRecord  (const string& id) {
        bool exists = student_list.nodeExists (id);

        if (exists) {
            Student data = getData(id);
            int padding = 15;

            UI::showLine ( UI::strPadEnd ( "Id", padding) + ": " + data.id );
            UI::showLine ( UI::strPadEnd ( "Name", padding) + ": " + data.name );
            UI::showLine ( UI::strPadEnd ( "Grade Level", padding) + ": " + to_string(data.grade_level) );
            UI::showLine ( UI::strPadEnd ( "City of birth", padding) + ": " + data.city_of_birth );
            
        } else {
            UI::showLine ("Not found.");
        }
    }

    // showTable related
    static void tableInitRows() {
        table_rows.clear();   
    }

    static void tableAddRow (Node<Student>* node ) {
        Student data = node->data;

        vector<string> row;
        row.push_back (data.id);
        row.push_back (data.name);
        row.push_back (to_string(data.grade_level));
        row.push_back (data.city_of_birth);

        // // debug
        // cout << endl << "DEBUG ROW" << endl;
        // for (size_t i = 0; i < row.size(); i++) {
        //     cout << row[i] << endl;
        // }

        table_rows.push_back(row); // add to rows
    }

    static void showTable(const string& title) {
        tableInitRows();

        // use anonymous callback function to build rows
        // add row one by one per node
        student_list.traverse ( tableAddRow );

        const string headers[] = {"Id", "Name", "Grade Level", "City of birth"};
        int col_sizes[] = {6, 20, 12, 30};
        int num_cols = 4;
    
        // exit(EXIT_SUCCESS);

        UI::showEmptyLine(1);
        UI::showTable(title, headers, table_rows, col_sizes, num_cols);
        UI::showEmptyLine(1);
    }

    // // this is optional, not needed if never used
    // // showNode as compact string line
    // static void showNodeAsLine (Node<Student>* node ) {
    //     Student data = node->data;
    //     string line = UI::strPadEnd(data.id,6) + 
    //         UI::strPadEnd(data.name, 15) +  
    //         UI::strPadEnd(to_string(data.grade_level),4) + 
    //         UI::strPadEnd(data.city_of_birth, 20);
    //     cout << line << endl; 
    // }

    // // simple compact list without header
    // static void showList () {
    //     student_list.traverse ( showNodeAsLine );
    // }
    
};
