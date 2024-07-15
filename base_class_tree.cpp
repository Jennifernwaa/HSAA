#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TreeNode {
public:
    string type;
    string id;
    string data;
    vector<TreeNode*> child;

    TreeNode(const string& type, const string& id, const string& data="")
        : type(type), id(id), data(data) {
    }

    void addChild(TreeNode* childNode) {
        child.push_back(childNode);
    }
};

class Tree {
public:

    // findNode: this will do a recursive search to find a node thta has type & id
    // usually you pass root (the topmost node) as parent
    static TreeNode* findNode(TreeNode* parent, const string& type, const string& id) {
        if (!parent)
            return nullptr;

        if (parent->type == type && parent->id == id)
            return parent;

        for (size_t i = 0; i < parent->child.size(); i++) {
            if (parent->child[i]->type == type && parent->child[i]->id == id) {
                return parent->child[i];
            }
        }

        for (size_t i = 0; i < parent->child.size(); i++) {
            TreeNode* found = findNode(parent->child[i], type, id);
            if (found)
                return found;
        }

        return nullptr;
    }

    // removeNode: this will remove a node and uts children
    // usually you pass root (the topmost node) as parent
    static void removeNode(TreeNode* parent, const string& type, const string& id) {
        if (!parent)
            return;

        for (size_t i = 0; i < parent->child.size(); ++i) {
            if (parent->child[i]->type == type && parent->child[i]->id == id) {
                // Remove the found node from the parent's child vector
                parent->child.erase(parent->child.begin() + i);
                return;
            }
            removeNode(parent->child[i], type, id); // Recursively search in the children
        }
    }

    // Traverse the tree and call the provided callback function
    static void traverse(TreeNode* root, void (*callback)(TreeNode*, int), int level = 0) {
        if (!root)
            return;

        callback(root, level);

        for (size_t i = 0; i < root->child.size(); i++) {
            traverse(root->child[i], callback, level + 1);
        }
    }

    // not needed but left here for reference
    // static void printPath(const vector<TreeNode*>& vec) {
    //     for (size_t i = 0; i < vec.size(); i++) {
    //         string type = vec[i]->type;
    //         string id = vec[i]->id;
    //         cout << "(" << type << ":" << id << ") ";
    //     }
    //     cout << endl;
    // }

    // static void printAllRootToLeafPaths(TreeNode* root, vector<TreeNode*>& vec) {
    //     if (!root)
    //         return;

    //     vec.push_back(root);

    //     if (root->child.empty()) {
    //         printPath(vec);
    //         vec.pop_back();
    //         return;
    //     }

    //     for (size_t i = 0; i < root->child.size(); i++)
    //         printAllRootToLeafPaths(root->child[i], vec);

    //     vec.pop_back();
    // }

    // static void printAllRootToLeafPaths(TreeNode* root) {
    //     if (!root)
    //         return;

    //     vector<TreeNode*> vec;
    //     printAllRootToLeafPaths(root, vec);
    // }
};