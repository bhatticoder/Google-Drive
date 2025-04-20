#pragma once
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
#define TABLE_SIZE 10
struct FileMetadata {
    std::string name;
    std::string type;
    int size;
    std::string date;
    std::string owner;
    std::string parentFolder; // New field to store the parent folder
    FileMetadata* next;

    FileMetadata(std::string name, std::string type, int size, std::string date, std::string owner, std::string parentFolder)
        : name(name), type(type), size(size), date(date), owner(owner), parentFolder(parentFolder), next(nullptr) {}
    FileMetadata(std::string name, std::string type, int size, std::string date, std::string owner);
};
int hashFunction(const std::string& key, int tableSize);
class HashTable {
private:
    FileMetadata** table; // Pointer to an array of pointers
    int capacity;
public:
    HashTable(); // Constructor
    ~HashTable(); // Destructor
    void insert(const std::string& name, const std::string& type,
        int size, const std::string& date, const std::string& owner);
    FileMetadata* search(const std::string& name);
    void remove(const std::string& name);
};
#endif
