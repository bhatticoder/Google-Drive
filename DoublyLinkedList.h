#pragma once
#include <string>
#include <iostream>

struct VersionNode {
    int versionNumber;          // Version number
    std::string content;        // File content for this version
    VersionNode* prev;          // Pointer to the previous version
    VersionNode* next;          // Pointer to the next version

    VersionNode(int versionNumber, const std::string& content)
        : versionNumber(versionNumber), content(content), prev(nullptr), next(nullptr) {
    }
};

class DoublyLinkedList {
private:
    VersionNode* head;          // Pointer to the first version
    VersionNode* tail;          // Pointer to the most recent version
    int versionCount;           // Total number of versions

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), versionCount(0) {}

    ~DoublyLinkedList() {
        while (head) {
            VersionNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Add a new version to the list
    void addVersion(const std::string& content) {
        versionCount++;
        VersionNode* newNode = new VersionNode(versionCount, content);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    // Rollback to a specific version
    void rollbackToVersion(int versionNumber) {
        VersionNode* current = tail;
        while (current && current->versionNumber != versionNumber) {
            VersionNode* temp = current;
            current = current->prev;
            delete temp; // Delete newer versions
        }
        if (current) {
            current->next = nullptr;
            tail = current;
            versionCount = versionNumber;
        }
        else {
            std::cout << "Version " << versionNumber << " not found.\n";
        }
    }

    // Display the version history
    void displayHistory() const {
        VersionNode* current = head;
        while (current) {
            std::cout << "Version " << current->versionNumber << ": " << current->content << "\n";
            current = current->next;
        }
    }

    // Get the content of the most recent version
    std::string getCurrentVersionContent() const {
        return tail ? tail->content : "No versions available.";
    }
};
