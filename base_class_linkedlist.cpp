#pragma once

#include <iostream>
#include <string>

using namespace std;

// Define a generic Node class
template <typename T>
class Node {
public:
    string id;
    T data;
    Node* next;

    Node(const string& _id, const T& _data) : id(_id), data(_data), next(nullptr) {}
};

// Define a generic LinkedList class
template <typename T>
class LinkedList {
private:
    Node<T>* head;

public:
    LinkedList() : head(nullptr) {}

    // Method to add a node to the linked list
    void addNode(const string& id, const T& data) {
        Node<T>* newNode = new Node<T>(id, data);
        if (!head) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Method to delete a node by id
    void deleteNode(const string& id) {
        if (!head) {
            return; // List is empty
        }

        if (head->id == id) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node<T>* current = head;
        while (current->next) {
            if (current->next->id == id) {
                Node<T>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return;
            }
            current = current->next;
        }
    }

    // Method to find a node by id
    Node<T>* findNode(const string& id) {
        Node<T>* current = head;
        while (current) {
            if (current->id == id) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // Node not found
    }

    // Method to check if a node with a given id exists
    bool nodeExists(const string& id) {
        return findNode(id) != nullptr;
    }

    // Method to traverse the list and apply a callback function
    void traverse(void (*callback)(Node<T>*)) {
        Node<T>* current = head;
        while (current) {
            callback(current);
            current = current->next;
        }
    }
};
