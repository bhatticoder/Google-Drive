#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class Graph {
private:
    // Adjacency list to represent user connections and shared files
    std::unordered_map<std::string, std::vector<std::string>> adjacencyList;

public:
    // Add a user to the graph
    void addUser(const std::string& user) {
        if (adjacencyList.find(user) == adjacencyList.end()) {
            adjacencyList[user] = {};
        }
    }

    // Share a file between two users
    void shareFile(const std::string& fromUser, const std::string& toUser, const std::string& fileName) {
        addUser(fromUser);
        addUser(toUser);
        adjacencyList[fromUser].push_back("Shared with " + toUser + ": " + fileName);
    }

    // View files shared by a user
    void viewSharedFiles(const std::string& user) const {
        auto it = adjacencyList.find(user);
        if (it == adjacencyList.end() || it->second.empty()) {
            std::cout << "No files shared by " << user << ".\n";
            return;
        }

        std::cout << "Files shared by " << user << ":\n";
        for (const auto& file : it->second) {
            std::cout << "- " << file << "\n";
        }
    }

    // Display all user connections
    void displayConnections() const {
        std::cout << "User Connections and Shared Files:\n";
        for (const auto& pair : adjacencyList) {
            std::cout << pair.first << ":\n";
            for (const auto& file : pair.second) {
                std::cout << "  - " << file << "\n";
            }
        }
    }
};
