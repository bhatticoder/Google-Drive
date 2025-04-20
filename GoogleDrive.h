#pragma once
#ifndef GOOGLEDRIVE_H
#define GOOGLEDRIVE_H
#include <string>
#include <queue>
#include "Queue.h"
#include "Stack.h"
#include "Graph.h"
#include "DoublyLinkedList.h"
#include "HashTable.h"
struct Node {
    std::string name;
    bool isFolder;
    Node* firstChild;
    Node* nextSibling;

    Node(const std::string& name, bool isFolder);
};

class GoogleDrive {
private:
    Node* root;
    Stack recycleBin; // Stack for recently deleted files
    Queue recentFiles; // Queue for recently accessed files
    Graph userGraph;   // Graph for user connections and file sharing

    // Helper recursive functions
    void displayTreeHelper(Node* node, int depth);
    void deleteTree(Node* node);
    Node* searchNodeHelper(Node* node, const std::string& name);

public:
    GoogleDrive();               // Constructor
    ~GoogleDrive();              // Destructor
    void addChild(const std::string& parentName, const std::string& name, bool isFolder);
    void displayTree();
    Node* searchNode(const std::string& name);
    void renameNode(const std::string& currentName, const std::string& newName);
    void deleteNode(const std::string& name);
    void restoreFile();
    Node* getRoot();
    Stack& getRecycleBin();
    Queue& getRecentFiles(); // Getter for the recent files queue
    Graph& getUserGraph();   // Getter for the user graph
};
#endif // GOOGLEDRIVE_H
