#include "googleDrive.h"
#include <iostream>
#include "Stack.h"
using namespace std;
Node::Node(const string& name, bool isFolder) {
    this->name = name;
    this->isFolder = isFolder;
    this->firstChild = nullptr;
    this->nextSibling = nullptr;
}
GoogleDrive::GoogleDrive() : recentFiles(5) { // Set the queue capacity to 5
    root = new Node("Root", true);
}
GoogleDrive::~GoogleDrive() {
    deleteTree(root);
}
void GoogleDrive::deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->firstChild);
    deleteTree(node->nextSibling);
    delete node;
}
Node* GoogleDrive::getRoot() {
    return root; // Return the root node
}
void GoogleDrive::addChild(const string& parentName, const string& name, bool isFolder) {
    // Find the parent node
    Node* parent = searchNode(parentName);
    if (!parent || !parent->isFolder) {
        cout << "Parent folder '" << parentName << "' not found or is not a folder!" << endl;
        return;
    }
    // Create the new node
    Node* newNode = new Node(name, isFolder);
    newNode->firstChild = nullptr;
    newNode->nextSibling = nullptr;
    // Add the new node as a child of the parent
    if (!parent->firstChild) {
        parent->firstChild = newNode;
    }
    else {
        Node* temp = parent->firstChild;
        while (temp->nextSibling) {
            temp = temp->nextSibling;
        }
        temp->nextSibling = newNode;
    }
}
void GoogleDrive::displayTree() {
    displayTreeHelper(root, 0);
}
void GoogleDrive::displayTreeHelper(Node* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << (node->isFolder ? "[Folder] " : "[File] ") << node->name << endl;
    displayTreeHelper(node->firstChild, depth + 1);
    displayTreeHelper(node->nextSibling, depth);
}
Node* GoogleDrive::searchNodeHelper(Node* node, const string& name) {
    if (!node) return nullptr;
    // Check if the current node matches the name
    if (node->name == name) return node;
    // Recursively search in the children
    Node* found = searchNodeHelper(node->firstChild, name);
    if (found) return found;
    // Recursively search in the siblings
    return searchNodeHelper(node->nextSibling, name);
}
void GoogleDrive::renameNode(const string& currentName, const string& newName) {
    Node* node = searchNode(currentName);
    if (node) {
        node->name = newName;
    }
    else {
        cout << "Node '" << currentName << "' not found!" << endl;
    }
}
void GoogleDrive::deleteNode(const std::string& name) {
    if (root->name == name) {
        cout << "Cannot delete the root folder!" << endl;
        return;
    }
    Node* parent = nullptr;
    Node* current = root;
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* temp = q.front();
        q.pop();
        Node* child = temp->firstChild;
        while (child) {
            if (child->name == name) {
                parent = temp;
                current = child;
                break;
            }
            q.push(child);
            child = child->nextSibling;
        }
        if (parent) break;
    }
    if (!parent) {
        cout << "Node '" << name << "' not found!" << endl;
        return;
    }
    // Push the file metadata onto the stack
    if (!current->isFolder) {
        recycleBin.push(new FileMetadata(current->name, "file", 0, "unknown", "unknown", parent->name));
    }
    // Remove the node from the parent's child list
    if (parent->firstChild == current) {
        parent->firstChild = current->nextSibling;
    }
    else {
        Node* sibling = parent->firstChild;
        while (sibling->nextSibling != current) {
            sibling = sibling->nextSibling;
        }
        sibling->nextSibling = current->nextSibling;
    }
    // Delete the subtree rooted at the current node
    deleteTree(current);
}
void GoogleDrive::restoreFile() {
    if (recycleBin.isEmpty()) {
        cout << "Recycle Bin is empty. No files to restore." << endl;
        return;
    }
    // Pop the most recently deleted file from the stack
    FileMetadata* file = recycleBin.pop();
    if (!file) return;
    // Restore the file to its original folder
    addChild(file->parentFolder, file->name, false);
    cout << "File '" << file->name << "' has been restored to folder '" << file->parentFolder << "'." << endl;
    delete file; // Free the memory for the file metadata
}
Stack& GoogleDrive::getRecycleBin() {
    return recycleBin;
}
Queue& GoogleDrive::getRecentFiles() {
    return recentFiles;
}
Node* GoogleDrive::searchNode(const std::string& name) {
    Node* result = searchNodeHelper(root, name);
    if (result && !result->isFolder) {
        // Add the file to the recent files queue
        recentFiles.enqueue(new FileMetadata(result->name, "file", 0, "unknown", "unknown", "unknown"));
    }
    return result;
}
Graph& GoogleDrive::getUserGraph() {
    return userGraph;
}
