#pragma once
#include "Stack.h" // For FileMetadata
#include<iostream>
using namespace std;
struct QueueNode {
    FileMetadata* file; // Pointer to the file metadata
    QueueNode* next;    // Pointer to the next node in the queue

    QueueNode(FileMetadata* file) : file(file), next(nullptr) {}
};

class Queue {
private:
    QueueNode* front; // Pointer to the front of the queue
    QueueNode* rear;  // Pointer to the rear of the queue
    int size;         // Current size of the queue
    int capacity;     // Maximum capacity of the queue

public:
    Queue(int capacity) : front(nullptr), rear(nullptr), size(0), capacity(capacity) {}

    ~Queue() {
        while (front) {
            QueueNode* temp = front;
            front = front->next;
            delete temp->file; // Free the file metadata
            delete temp;       // Free the queue node
        }
    }

    // Add a file to the queue
    void enqueue(FileMetadata* file) {
        // If the queue is full, remove the oldest file
        if (size == capacity) {
            dequeue();
        }

        QueueNode* newNode = new QueueNode(file);
        if (!rear) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    // Remove the oldest file from the queue
    void dequeue() {
        if (!front) return; // Queue is empty

        QueueNode* temp = front;
        front = front->next;
        if (!front) rear = nullptr; // If the queue becomes empty
        delete temp->file;          // Free the file metadata
        delete temp;                // Free the queue node
        size--;
    }
    // Display the contents of the queue
    void display() const {
        QueueNode* current = front;
        while (current) {
            cout << "File: " << current->file->name << ", Type: " << current->file->type << endl;
            current = current->next;
        }
    }
    // Check if the queue is empty
    bool isEmpty() const {
        return size == 0;
    }
};
