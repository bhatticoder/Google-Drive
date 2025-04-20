#pragma once
#include "HashTable.h"
struct StackNode {
    FileMetadata* file; // Pointer to the file metadata
    StackNode* next;    // Pointer to the next node in the stack

    StackNode(FileMetadata* file) : file(file), next(nullptr) {}
};
class Stack {
private:
    StackNode* top; // Pointer to the top of the stack
public:
    Stack() : top(nullptr) {}

    ~Stack() {
        while (top) {
            StackNode* temp = top;
            top = top->next;
            delete temp->file; // Free the file metadata
            delete temp;       // Free the stack node
        }
    }
    // Push a file onto the stack
    void push(FileMetadata* file) {
        StackNode* newNode = new StackNode(file);
        newNode->next = top;
        top = newNode;
    }
    // Pop a file from the stack
    FileMetadata* pop() {
        if (!top) return nullptr; // Stack is empty
        StackNode* temp = top;
        top = top->next;
        FileMetadata* file = temp->file;
        delete temp; // Free the stack node
        return file;
    }
    // Peek at the top file without removing it
    FileMetadata* peek() const {
        return top ? top->file : nullptr;
    }
    // Check if the stack is empty
    bool isEmpty() const {
        return top == nullptr;
    }
};
