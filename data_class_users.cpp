#pragma once

// we should hardcode one "admin" user
// and also some sample parents of students
// for this app we only manage one student per parent
// use student data from data_class_student a

#pragma once

#include "base_class_linkedlist.cpp"
#include "base_class_ui.cpp"
#include "data_class_students.cpp"

using namespace std;

// table_rows placeholder for showTable related (used in traverse in addTableRow)
// wrapped in IF because it's also defined in other data classes
#ifndef _TABLE_ROWS_FOR_DISPLAY
vector<vector<string> > table_rows; 
#define _TABLE_ROWS_FOR_DISPLAY
#endif

// Define a simple struct as an example of data
struct User {
    string id;
    string name;
    string password;
    string user_type;
    string student_id;
};

LinkedList<User> user_list;
   
// this is to be returned after login (excluding password)
struct UserInfo {
    bool valid; // true if valid login
    string message; // may contain error message
    string id;
    string name;
    string user_type;
    string student_id;
};

class UserList {

public:

    // add data for quick test
    static void init() {

        // Create initial data nodes
        // admins
        addNew("admin", "Administrator", "adminpwd", "admin", "");

        // parents
        addNew("P001", "Parent of S001", "p001", "parent", "S001");
    }

    // add new 
    static bool addNew (const string& id, const string& name, const string& password, const string& user_type, const string& student_id) {
        bool exists = user_list.nodeExists (id);

        if (!exists) {
            User data = {id, name, password, user_type, student_id};
            user_list.addNode (id, data);
            return true;
        } else {
            return false; // node with the same id already exists
        }
    }

    // delete existing

    static bool removeUser(const string& id) {
        bool exists = user_list.nodeExists(id);

        if (exists) {
            user_list.deleteNode(id);
            return true;
        } else {
            return false; // node with that ID does not exist
        }
    }

    static void removeUserById() {
        char choice;
        do {
            string idToRemove;
            cout << "Enter the ID of the user you want to remove: ";
            cin >> idToRemove;

            bool removed = removeUser(idToRemove);

            if (removed) {
                cout << "User removed successfully." << endl;
            } else {
                cout << "User with the specified ID not found." << endl;
            }

            cout << "Do you want to delete more users? (y/n): ";
            cin >> choice;

        } while (choice == 'y' || choice == 'Y');
    }
 // get data
    static User getData (const string& id) {
        bool exists = user_list.nodeExists (id);
        Node<User>* node;
        User data;
        if (exists) {
            node = user_list.findNode(id);
            data = node->data;
        }
        return data;
    }
    // update existing user

    static bool userExists(const string& id) {
        return user_list.nodeExists(id);
    }

    // Update user information
    static void updateUser(const string& id, const string& newId, const string& name, const string& password, const string& user_type, const string& student_id) {
        // Check if the user with the given ID exists
        if (user_list.nodeExists(id)) {
            // Remove the existing user data
            user_list.deleteNode(id);

            // Add the updated user data
            User data = {newId, name, password, user_type, student_id};
            user_list.addNode(newId, data);
        } else {
            cout << "User with ID " << id << " not found." << endl;
        }
    }

    // showTable related
    static void tableInitRows() {
        table_rows.clear();   
    }

    static void tableAddRow (Node<User>* node ) {
        User data = node->data;

        vector<string> row;
        row.push_back (data.id);
        row.push_back (data.name);
        row.push_back (data.password);
        row.push_back (data.user_type);
        row.push_back (data.student_id);

        table_rows.push_back(row); // add to rows
    }

    static void showTable(const string& title) {
        tableInitRows();

        // use anonymous callback function to build rows
        // add row one by one per node
        user_list.traverse ( tableAddRow );

        const string headers[] = {"Id", "Name", "Password", "User Type", "Student Id"};
        int col_sizes[] = {6, 30, 15, 10, 10};
        int num_cols = 5;
    
        // exit(EXIT_SUCCESS);

        UI::showEmptyLine(1);
        UI::showTable(title, headers, table_rows, col_sizes, num_cols);
        UI::showEmptyLine(1);
    }
    
    // login related: returns user_type
    static UserInfo login(const string& id, const string& password) {
        bool exists = user_list.nodeExists (id);

        // init response
        UserInfo info;
        info.valid = false; // set to false
        info.message = "";
        info.name = "";
        info.user_type = "";
        info.student_id = "";

        if (exists) {
            Node<User>* node = user_list.findNode(id);
            User data = node->data;

            if (password == data.password) {

                info.valid = true;
                info.message = "Success";
                info.id = data.id;
                info.name = data.name;
                info.user_type = data.user_type;
                info.student_id = data.student_id;

            } else {
                info.valid = false;
                info.message = "Wrong password";
            }

        } else {
            info.valid = false;
            info.message = "User does not exists";
        }


        return info;
    }
    
};