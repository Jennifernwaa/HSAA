#pragma once

#include "base_class_linkedlist.cpp"

using namespace std;

// simple class defined to retreive name (for display etc)

// Define a simple struct as an example of data
struct GradeLevel {
    string id;
    string name;
};

LinkedList<GradeLevel> gradelevel_list;
   
class GradeLevelList {

public:

    // add data for quick test
    static void init() {

        // Create initial data nodes
        addNew("G10", "Grade 10");
        addNew("G11", "Grade 11");
        addNew("G12", "Grade 12");
    }

    // add new 
    static bool addNew (const string& id, const string& name) {
        bool exists = gradelevel_list.nodeExists (id);

        if (!exists) {
            GradeLevel data = {id, name};
            gradelevel_list.addNode (id, data);
            
            // should add course to tree (under student)
            
            return true;
        } else {
            return false; // node with the same id already exists
        }
    }

    // exists
    static bool exists (const string& id) {
        return gradelevel_list.nodeExists (id);
    }

    // get data
    static GradeLevel getData (const string& id) {
        bool exists = gradelevel_list.nodeExists (id);
        Node<GradeLevel>* node;
        GradeLevel data;
        if (exists) {
            node = gradelevel_list.findNode(id);
            data = node->data;
        }
        return data;
    }
    
};