#include "googleDrive.h"
#include "HashTable.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // Unix/Linux/Mac
    #endif
}
void printSuccess(const string& message) {
    cout << GREEN << message << RESET << endl;
}
void printError(const string& message) {
    cout << RED << message << RESET << endl;
}
void printPrompt(const string& message) {
    cout << YELLOW << message << RESET << endl;
}
void showLoading(const string& message) {
    cout << message;
    for (int i = 0; i < 4; ++i) {
        cout << ".";
        this_thread::sleep_for(chrono::milliseconds(500)); // Add a delay
    }
    cout << endl;
}
void showBreadcrumb(const string& currentPath) {
    cout << BLUE << "Current Path: " << currentPath << RESET << endl;
}
void displayMenu() {
    cout << "=====================================\n";
    cout << "         Google Drive Menu           \n";
    cout << "=====================================\n";
    cout << "1. Add Folder\n";
    cout << "2. Add File\n";
    cout << "3. Rename\n";
    cout << "4. Delete\n";
    cout << "5. Search\n";
    cout << "6. Display Structure\n";
    cout << "7. View Recycle Bin\n";
    cout << "8. Restore File\n";
    cout << "9. Show Help\n";
    cout << "10. View Recent Files\n";
    cout << "11. Add File Version\n";
    cout << "12. Rollback File Version\n";
    cout << "13. View File Version History\n";
    cout << "14. Exit\n"; // Exit is now the last option
    cout << "=====================================\n";
    cout << "Enter your Choice: ";
}
void showHelp() {
    clearScreen();
    cout << "=====================================\n";
    cout << "              Help Menu              \n";
    cout << "=====================================\n";
    cout << "1. Add Folder: Add a new folder to the structure.\n";
    cout << "2. Add File: Add a new file to a folder.\n";
    cout << "3. Rename: Rename a file or folder.\n";
    cout << "4. Delete: Delete a file or folder.\n";
    cout << "5. Search: Search for a file by name.\n";
    cout << "6. Display Structure: Show the folder structure.\n";
    cout << "7. Exit: Exit the program.\n";
    cout << "8. View Recycle Bin: View the most recently deleted file.\n";
    cout << "9. Restore File: Restore the most recently deleted file.\n";
    cout << "10. Show Help: Display this help menu.\n";
    cout << "=====================================\n";
    cout << "Press Enter to go back to the main menu...";
    cin.ignore();
    cin.get();
}
bool confirmAction(const string& action) {
    char choice;
    cout << YELLOW << "Are you sure you want to " << action << "? (y/n): " << RESET;
    cin >> choice;
    return (choice == 'y' || choice == 'Y');
}
int getValidatedChoice() {
    int choice;
    while (true) {
        cout << "Enter your choice: ";
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 14) { // Updated range
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            printError("Invalid choice! Please enter a number between 1 and 14.");
        } else {
            return choice;
        }
    }
}
void displayTreeWithSymbols(Node* node, int depth = 0) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << (node->isFolder ? "[📁] " : "[📄] ") << node->name << endl;
    displayTreeWithSymbols(node->firstChild, depth + 1);
    displayTreeWithSymbols(node->nextSibling, depth);
}
int main() {
    GoogleDrive drive;
    HashTable hashTable; // Initialize the hash table
    int choice;
    bool exitProgram = false;

    do {
        clearScreen(); // Clear the screen before displaying the menu
        displayMenu();
        choice = getValidatedChoice();

        switch (choice) {
        case 1: { // Add Folder
            bool goBack = false;
            do {
                clearScreen();
                string parentName, folderName;
                cout << "Enter parent folder name: ";
                cin >> parentName;
                cout << "Enter folder name: ";
                cin >> folderName;
                drive.addChild(parentName, folderName, true);
                printPrompt("Do you want to go back to the main menu? (y/n): ");
                char backChoice;
                cin >> backChoice;
                if (backChoice == 'y' || backChoice == 'Y') goBack = true;
                else if (backChoice == 'n' || backChoice == 'N') break;
            } while (!goBack);
            break;
        }
        case 2: { // Add File
            bool goBack = false;
            do {
                clearScreen();
                string parentName, fileName, fileType, fileDate, fileOwner;
                int fileSize;
                cout << "Enter parent folder name: ";
                cin >> parentName;
                cout << "Enter file name: ";
                cin >> fileName;
                cout << "Enter file type: ";
                cin >> fileType;
                cout << "Enter file size (in KB): ";
                cin >> fileSize;
                cout << "Enter file creation date: ";
                cin >> fileDate;
                cout << "Enter file owner: ";
                cin >> fileOwner;
                drive.addChild(parentName, fileName, false);
                hashTable.insert(fileName, fileType, fileSize, fileDate, fileOwner);
                printSuccess("File added successfully!");
                printPrompt("Do you want to go back to the main menu? (y/n): ");
                char backChoice;
                cin >> backChoice;
                if (backChoice == 'y' || backChoice == 'Y') goBack = true;
                else if (backChoice == 'n' || backChoice == 'N') break;
            } while (!goBack);
            break;
        }
        case 3: { // Rename
            bool goBack = false;
            do {
                clearScreen();
                string oldName, newName;
                cout << "Enter current name: ";
                cin >> oldName;
                cout << "Enter new name: ";
                cin >> newName;
                Node* node = drive.searchNode(oldName);
                if (node) {
                    if (!node->isFolder) {
                        FileMetadata* metadata = hashTable.search(oldName);
                        if (metadata) {
                            hashTable.remove(oldName);
                            hashTable.insert(newName, metadata->type, metadata->size, metadata->date, metadata->owner);
                        }
                    }
                    drive.renameNode(oldName, newName);
                } else {
                    printError("File not found!");
                }
                printPrompt("Do you want to go back to the main menu? (y/n): ");
                char backChoice;
                cin >> backChoice;
                if (backChoice == 'y' || backChoice == 'Y') goBack = true;
                else if (backChoice == 'n' || backChoice == 'N') break;
            } while (!goBack);
            break;
        }
        case 4: { // Delete
            bool goBack = false;
            do {
                clearScreen();
                string name;
                cout << "Enter name to delete: ";
                cin >> name;
                Node* node = drive.searchNode(name);
                if (node) {
                    if (!node->isFolder) {
                        hashTable.remove(name);
                    }
                    if (confirmAction("delete this file")) {
                        drive.deleteNode(name);
                        printSuccess("File deleted successfully!");
                    } else {
                        printPrompt("Action canceled.");
                    }
                } else {
                    printError("File not found!");
                }
                printPrompt("Do you want to go back to the main menu? (y/n): ");
                char backChoice;
                cin >> backChoice;
                if (backChoice == 'y' || backChoice == 'Y') goBack = true;
                else if (backChoice == 'n' || backChoice == 'N') break;
            } while (!goBack);
            break;
        }
        case 5: { // Search
            bool goBack = false;
            do {
                clearScreen();
                string name;
                cout << "Enter name to search: ";
                cin >> name;
                showLoading("Searching for the file");
                FileMetadata* metadata = hashTable.search(name);
                if (metadata) {
                    cout << "File found:\n";
                    cout << "Name: " << metadata->name << "\n";
                    cout << "Type: " << metadata->type << "\n";
                    cout << "Size: " << metadata->size << " KB\n";
                    cout << "Date: " << metadata->date << "\n";
                    cout << "Owner: " << metadata->owner << "\n";
                } else {
                    printError("File not found!");
                }
                printPrompt("Do you want to go back to the main menu? (y/n): ");
                char backChoice;
                cin >> backChoice;
                if (backChoice == 'y' || backChoice == 'Y') goBack = true;
                else if (backChoice == 'n' || backChoice == 'N') break;
            } while (!goBack);
            break;
        }
        case 6: { // Display Structure
            bool goBack = false;
            do {
                clearScreen();
                displayTreeWithSymbols(drive.getRoot());
                printPrompt("Do you want to go back to the main menu? (y/n): ");
                char backChoice;
                cin >> backChoice;
                if (backChoice == 'y' || backChoice == 'Y') goBack = true;
                else if (backChoice == 'n' || backChoice == 'N') break;
            } while (!goBack);
            break;
        }
        case 7: { // View Recycle Bin
            if (drive.getRecycleBin().isEmpty()) {
                printError("Recycle Bin is empty.");
            } else {
                FileMetadata* file = drive.getRecycleBin().peek();
                cout << "Most recently deleted file: " << file->name << endl;
            }
            printPrompt("Press Enter to return to the main menu...");
            cin.ignore();
            cin.get();
            break;
        }
        case 8: { // Restore File
            drive.restoreFile();
            printPrompt("Press Enter to return to the main menu...");
            cin.ignore();
            cin.get();
            break;
        }
        case 9: { // Show Help
            showHelp();
            break;
        }
        case 10: { // View Recent Files
            clearScreen();
            cout << "Displaying recent files...\n";
            // Placeholder for actual implementation
            printPrompt("Press Enter to return to the main menu...");
            cin.ignore();
            cin.get();
            break;
        }
        case 11: { // Add File Version
            clearScreen();
            cout << "Adding file version...\n";
            // Placeholder for actual implementation
            printPrompt("Press Enter to return to the main menu...");
            cin.ignore();
            cin.get();
            break;
        }
        case 12: { // Rollback File Version
            clearScreen();
            cout << "Rolling back file version...\n";
            // Placeholder for actual implementation
            printPrompt("Press Enter to return to the main menu...");
            cin.ignore();
            cin.get();
            break;
        }
        case 13: { // View File Version History
            clearScreen();
            cout << "Viewing file version history...\n";
            // Placeholder for actual implementation
            printPrompt("Press Enter to return to the main menu...");
            cin.ignore();
            cin.get();
            break;
        }
        case 14: // Exit
            cout << "Exiting...\n";
            exitProgram = true;
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (!exitProgram);
    return 0;
}
